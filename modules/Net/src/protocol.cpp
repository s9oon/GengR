#include "GengR/Net/protocol.hpp"

namespace GengR::Net {
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

    void dispatchPacket(_ENetPeer* from, const std::uint8_t *data, std::size_t length, PacketHandler& handler) {
        if (length < 1) {
            return; // never trust a packet: too short to even have a type byte
        }

        switch (static_cast<PacketType>(data[0])) {
            case PacketType::Message:
                handler.onMessage(from, deserializeMessage(data, length));
                break;
            default:
                break; // unknown type: ignore it
        }
    }
}
