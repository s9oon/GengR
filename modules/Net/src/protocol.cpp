#include "GengR/Net/protocol.hpp"
#include <cstring>

namespace GengR::Net {

    // this is vulnrable so im gonna leave it here for exploiting later
    // Real problems like edian assumptions

    std::vector<std::uint8_t> serializePosition(Vec2 pos) {
        std::vector<std::uint8_t> bytes(9);                   
        bytes[0] = static_cast<std::uint8_t>(PacketType::Position);
        std::memcpy(&bytes[1], &pos.x, 4);
        std::memcpy(&bytes[5], &pos.y, 4); 
        return bytes;
    }

    // data/length = the whole packet, type byte included
    std::optional<Vec2> deserializePosition(const std::uint8_t* data, std::size_t length) {
        if (length != 9) {
            return std::nullopt; // never trust a packet: wrong size
        }

        Vec2 pos;
        std::memcpy(&pos.x, data + 1, 4); // bytes 1-4 → x
        std::memcpy(&pos.y, data + 5, 4); // bytes 5-8 → y
        return pos;
    }

    // [type][id][x][y] = 1 + 4 + 4 + 4 = 13 bytes
    std::vector<std::uint8_t> serializePlayerPosition(std::uint32_t id, Vec2 pos) {
        std::vector<std::uint8_t> bytes(13);
        bytes[0] = static_cast<std::uint8_t>(PacketType::PlayerPosition);
        std::memcpy(&bytes[1], &id, 4);    // bytes 1-4  → id
        std::memcpy(&bytes[5], &pos.x, 4); // bytes 5-8  → x
        std::memcpy(&bytes[9], &pos.y, 4); // bytes 9-12 → y
        return bytes;
    }

    // data/length = the whole packet, type byte included
    std::optional<PlayerPosition> deserializePlayerPosition(const std::uint8_t* data, std::size_t length) {
        if (length != 13) {
            return std::nullopt; // never trust a packet: wrong size
        }

        PlayerPosition result;
        std::memcpy(&result.id, data + 1, 4);    // bytes 1-4  → id
        std::memcpy(&result.pos.x, data + 5, 4); // bytes 5-8  → x
        std::memcpy(&result.pos.y, data + 9, 4); // bytes 9-12 → y
        return result;
    }

    // [type][text...]
    std::vector<std::uint8_t> serializeMessage(const std::string& text) {
        std::vector<std::uint8_t> bytes;
        bytes.reserve(1 + text.size()); // one allocation, exact size

        bytes.push_back(static_cast<std::uint8_t>(PacketType::Message)); // byte 0
        bytes.insert(bytes.end(), text.begin(), text.end());              // bytes 1..

        return bytes;
    }

    // data/length = the whole packet, type byte included; the caller has already checked the type
    std::string deserializeMessage(const std::uint8_t *data, std::size_t length) {
        if (length < 1) {
            return {}; // never trust a packet: too short to even have a type byte
        }

        // everything after the type byte is the text
        return std::string(reinterpret_cast<const char*>(data + 1), length - 1);
    }

    // packets a client may send to the server
    void dispatchServerPacket(_ENetPeer* from, const std::uint8_t *data, std::size_t length, ServerPacketHandler& handler) {
        if (length < 1) {
            return; // never trust a packet: too short to even have a type byte
        }

        switch (static_cast<PacketType>(data[0])) {
            case PacketType::Message:
                handler.onMessage(from, deserializeMessage(data, length));
                break;
            case PacketType::Position:
                // only call the handler if the packet was valid (exactly 9 bytes)
                if (auto pos = deserializePosition(data, length)) {
                    handler.onPosition(from, *pos);
                }
                break;
            default:
                break; // unknown or server->client type: ignore it
        }
    }

    // packets the server may send to a client
    void dispatchClientPacket(_ENetPeer* from, const std::uint8_t *data, std::size_t length, ClientPacketHandler& handler) {
        if (length < 1) {
            return; // never trust a packet: too short to even have a type byte
        }

        switch (static_cast<PacketType>(data[0])) {
            case PacketType::Message:
                handler.onMessage(from, deserializeMessage(data, length));
                break;
            case PacketType::PlayerPosition:
                // only call the handler if the packet was valid (exactly 13 bytes)
                if (auto player = deserializePlayerPosition(data, length)) {
                    handler.onPlayerPosition(player->id, player->pos);
                }
                break;
            default:
                break; // unknown or client->server type: ignore it
        }
    }
}
