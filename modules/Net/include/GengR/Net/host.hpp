#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include "GengR/Net/types.hpp"

// forward declared so this header doesn't need enet.h
struct _ENetHost;
struct _ENetEvent;
struct _ENetPeer;

namespace GengR::Net {

    struct Address {
        std::string host;
        std::uint16_t port = 0;
    };

    // Owns one ENet host: sets ENet up when created, cleans it up when destroyed.
    // Server and Client both build on this.
    class Host {
    public:
        // bind = the address to listen on (server), or std::nullopt to only connect out (client)
        // throws std::runtime_error on failure
        Host(const std::optional<Address>& bind,
             std::size_t maxPeers,
             std::size_t maxChannels,
             std::uint32_t incomingBandwidth,
             std::uint32_t outgoingBandwidth);
            
        std::vector<_ENetEvent> poll(std::uint8_t tick);

        // serializes text as a Message packet and queues it for peer; it goes out on the next poll
        void sendMessage(_ENetPeer* peer, const std::string &text);
        void sendPosition(_ENetPeer* peer, const Vec2 &position);

        ~Host();

        // one Host owns one ENet host, so no copying or moving
        Host(const Host&) = delete;
        Host& operator=(const Host&) = delete;

        // the raw ENet host, for Server/Client code that calls ENet directly
        _ENetHost* get() const { return host; }

    private:
        void send(_ENetPeer* peer, const std::vector<std::uint8_t>& bytes, std::uint8_t channel, bool reliable);

        _ENetHost* host = nullptr;
    };

}
