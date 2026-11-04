#include <enet.h>
#include "GengR/Server/server.hpp"
#include <print>

namespace GengR::Server {
    Host::Host(const Config& config)
        : host(Net::Address{ config.host, config.port },
               config.maxPeers,
               config.maxChannels,
               config.incomingBandwidth,
               config.outgoingBandwidth) {
    }

    void Host::start() {
        running = true;

        while (running) {
            pollEvents();
        }
    }

    void Host::stop() {
        running = false;
    }

    void Host::pollEvents() {
        auto events = host.poll(10);
        for (auto event : events) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    Net::dispatchPacket(event.peer, event.packet->data, event.packet->dataLength, *this);
                    enet_packet_destroy(event.packet); // must always be destroyed
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    break;
                case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
                    break;
                default:
                    break;
            }
        }
    }

    void Host::onMessage(_ENetPeer* from, const std::string& text) {
        std::println("message: {}", text);
    }
}

/*
#include <iostream>

int main() {
    ENetAddress address;
    ENetHost *server;

    address = {
        ENET_HOST_ANY, // Set to 0.0.0.0
        1234 // Set the port of 1234
    };

    server = enet_host_create(&address, 
        16, // Allow 16 client connections
        2, // Allow 2 channels
        0, // Allow unlimited incoming bandwidth
        0 // ALlow unlimited outcoming bandwidth
    );

    if (server == NULL) {
        std::cerr << "Failed to create ENet server!\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Press any key to quit...\n";

    std::cin.get();

    return 0;
}
*/