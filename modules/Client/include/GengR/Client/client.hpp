#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include "GengR/Net/host.hpp"
#include "GengR/Net/protocol.hpp"

struct _ENetPeer;

namespace GengR::Client {

    struct Config {
        std::size_t maxChannels = 2; // must match the server's

        std::uint32_t incomingBandwidth = 0;
        std::uint32_t outgoingBandwidth = 0;
    };

    class Host : public Net::ClientPacketHandler {
    public:
        explicit Host(const Config& config); // throws std::runtime_error on failure

        // starts connecting; isConnected() turns true once pollEvents() sees the CONNECT event
        // throws std::runtime_error if the address is bad
        void connectToServer(const Net::Address &address);

        void sendMessage(const std::string& text);
        void sendPosition(const Vec2 &position);

        // handles whatever arrived; 0 = don't wait (for a game loop)
        void pollEvents(std::uint8_t timeoutMs = 0);

        bool isConnected() const { return connected; }

        _ENetPeer* getPeer() const { return peer; } // the connection to the server, nullptr if none

        // id → latest position of every other player we've heard from
        // const& so main reads it without copying; const so main can't change it
        const std::unordered_map<std::uint32_t, Vec2>& otherPlayers() const { return others; }

        // one Host owns one ENet host, so no copying or moving
        Host(const Host&) = delete;
        Host& operator=(const Host&) = delete;

    private:
        // ClientPacketHandler: what the client does with each packet the server can send
        void onMessage(_ENetPeer* from, const std::string& text) override;
        void onPlayerPosition(std::uint32_t id, Vec2 pos) override;

        Net::Host host; // cleans itself up, so Client::Host needs no destructor
        std::size_t maxChannels; // needed again when connecting
        _ENetPeer *peer = nullptr;
        bool connected = false;
        std::unordered_map<std::uint32_t, Vec2> others; // filled by onPlayerPosition
    };

}
