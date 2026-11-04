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