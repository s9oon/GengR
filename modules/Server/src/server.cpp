#include <enet.h>
#include "GengR/Server/server.hpp"

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
        ENetEvent event;

        // waits up to 10ms for an event, so the loop in start() doesn't spin
        while (enet_host_service(host.get(), &event, 10) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
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