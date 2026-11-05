#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <optional>
#include "GengR/Net/types.hpp"

// forward declared so this header doesn't need enet.h
struct _ENetPeer;

namespace GengR::Net {
    enum class PacketType : std::uint8_t {
        Message = 1,
        Position = 2,       // client -> server: [type][x][y]
        PlayerPosition = 3, // server -> client: [type][id][x][y]
    };

    std::vector<std::uint8_t> serializePosition(Vec2 pos);
    std::optional<Vec2> deserializePosition(const std::uint8_t *data, std::size_t length);

    std::vector<std::uint8_t> serializePlayerPosition(std::uint32_t id, Vec2 pos);

    std::vector<std::uint8_t> serializeMessage(const std::string& text);
    std::string deserializeMessage(const std::uint8_t *data, std::size_t length);

    // Packets are split by direction. Each side only has handlers for what it can legitimately
    // receive, so a packet sent the wrong way (e.g. a forged one) has no handler and is ignored.

    // client -> server: what a server can receive; Server::Host inherits this
    class ServerPacketHandler {
    public:
        virtual ~ServerPacketHandler() = default;

        virtual void onMessage(_ENetPeer* from, const std::string& text) = 0;
        virtual void onPosition(_ENetPeer* from, Vec2 pos) = 0; // by value: Vec2 is only 8 bytes
    };

    // server -> client: what a client can receive; Client::Host inherits this
    class ClientPacketHandler {
    public:
        virtual ~ClientPacketHandler() = default;

        virtual void onMessage(_ENetPeer* from, const std::string& text) = 0;
        virtual void onPlayerPosition(std::uint32_t id, Vec2 pos) = 0;
    };

    // read the type byte, parse the packet, and call the matching method on handler
    // data/length = the whole packet; bad, unknown or wrong-direction packets are ignored
    void dispatchServerPacket(_ENetPeer* from, const std::uint8_t *data, std::size_t length, ServerPacketHandler& handler);
    void dispatchClientPacket(_ENetPeer* from, const std::uint8_t *data, std::size_t length, ClientPacketHandler& handler);
}