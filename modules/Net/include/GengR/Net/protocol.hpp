#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>

// forward declared so this header doesn't need enet.h
struct _ENetPeer;

namespace GengR::Net {
    enum class PacketType : std::uint8_t {
        Message = 1,
    };

    std::vector<std::uint8_t> serializeMessage(const std::string& text);
    std::string deserializeMessage(const std::uint8_t *data, std::size_t length);

    // what can arrive; Server and Client inherit this and must define every method (= 0)
    class PacketHandler {
    public:
        virtual ~PacketHandler() = default;

        virtual void onMessage(_ENetPeer* from, const std::string& text) = 0;
    };

    // reads the type byte, parses the packet, and calls the matching method on handler
    // data/length = the whole packet; bad or unknown packets are ignored
    void dispatchPacket(_ENetPeer* from, const std::uint8_t *data, std::size_t length, PacketHandler& handler);
}