#include <enet.h>
#include "GengR/Client/client.hpp"
#include <format>
#include <print>
#include <stdexcept>

namespace GengR::Client {
    Host::Host(const Config& config)
        : host(std::nullopt, // no bind address: a client only connects out
               1,            // only 1 outgoing connection, to the server
               config.maxChannels,
               config.incomingBandwidth,
               config.outgoingBandwidth),
          maxChannels(config.maxChannels) {
    }

    void Host::connectToServer(const Net::Address &address) {
        ENetAddress enetAddress{};

        enetAddress.port = address.port;

        if (enet_address_set_host(&enetAddress, address.host.c_str()) != 0) {
            throw std::runtime_error(std::format("couldn't set host '{}'", address.host));
        }

        // only starts the connection; the CONNECT event in pollEvents() says when it's done
        peer = enet_host_connect(host.get(), &enetAddress, maxChannels, 0);

        if (peer == nullptr) {
            throw std::runtime_error(std::format("couldn't start connecting to [{}]:{}", address.host, address.port));
        }
    }

    void Host::sendMessage(const std::string &text) {
        if (!connected) {
           throw std::runtime_error("not connected to a server");
        }

        host.sendMessage(peer, text);
    }

    void Host::sendPosition(const Vec2 &position) {
        if (!connected) {
            return; // called every frame, including before the connection is up, so don't throw
        }

        host.sendPosition(peer, position);
    }

    void Host::pollEvents(std::uint8_t timeoutMs) {
        auto events = host.poll(timeoutMs);
        for (auto event : events) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    connected = true;
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    Net::dispatchPacket(event.peer, event.packet->data, event.packet->dataLength, *this);
                    enet_packet_destroy(event.packet); // must always be destroyed
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT: // also what a failed connection attempt ends in
                    connected = false;
                    peer = nullptr;
                    break;
                default:
                    break;
            }
        }
    }

    void Host::onMessage(_ENetPeer* from, const std::string& text) {
        std::println("message from server: {}", text);
    }

    void Host::onPosition(_ENetPeer* from, Vec2 pos) {
        // TODO
    }
}
