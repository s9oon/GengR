#pragma once

#include <string>
#include <cstdint>
#include <cstddef>
#include <atomic>
#include "GengR/Net/host.hpp"
#include "GengR/Net/protocol.hpp"

namespace GengR::Server {

    struct Config {
        std::string host = "::";
        std::uint16_t port = 1234; // this is a 16 bit unsigned interger and what is the t for and how does it defer from the ones below?

        std::size_t maxPeers = 32;
        std::size_t maxChannels = 2;

        std::uint32_t incomingBandwidth = 0;
        std::uint32_t outgoingBandwidth = 0;
    };
    
    class Host : public Net::PacketHandler {
    public:
        explicit Host(const Config& config); // throws std::runtime_error on failure

        // one Host owns one ENet host, so no copying or moving
        Host(const Host&) = delete;
        Host& operator=(const Host&) = delete;

        void start(); // blocks until stop() is called
        void stop();

    private:
        void pollEvents();

        // PacketHandler: what the server does with each packet type
        void onMessage(_ENetPeer* from, const std::string& text) override;
        void onPosition(_ENetPeer* from, Vec2 pos) override;

        void broadcast(_ENetPeer* from, const std::string& text);

        std::vector<_ENetPeer*> peers; 
        Net::Host host; // cleans itself up, so Server::Host needs no destructor
        std::atomic<bool> running = false;
    };

}