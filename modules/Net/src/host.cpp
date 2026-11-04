#include <enet.h>
#include "GengR/Net/host.hpp"
#include <format>
#include <stdexcept>

namespace GengR::Net {
    Host::Host(const std::optional<Address>& bind,
               std::size_t maxPeers,
               std::size_t maxChannels,
               std::uint32_t incomingBandwidth,
               std::uint32_t outgoingBandwidth) {
        if (enet_initialize() != 0) {
            throw std::runtime_error("couldn't initialize ENet");
        }

        ENetAddress address{};

        if (bind) {
            address.port = bind->port;

            if (enet_address_set_host(&address, bind->host.c_str()) != 0) {
                enet_deinitialize();
                throw std::runtime_error(std::format("couldn't set host '{}'", bind->host));
            }
        }

        // no bind address = a client host that only connects out
        host = enet_host_create(bind ? &address : nullptr,
            maxPeers,
            maxChannels,
            incomingBandwidth,
            outgoingBandwidth
        );

        if (host == nullptr) {
            enet_deinitialize();

            if (bind) {
                throw std::runtime_error(std::format("couldn't create host on [{}]:{}", bind->host, bind->port));
            }
            throw std::runtime_error("couldn't create client host");
        }
    }

    std::vector<_ENetEvent> Host::poll(std::uint8_t tick) {
        std::vector<_ENetEvent> events; // new, empty list each call
        _ENetEvent event;

        while(enet_host_service (host, &event, tick) > 0) {
            events.push_back(event);
        }

        return events;
    }

    // runs automatically when the Host goes out of scope
    Host::~Host() {
        enet_host_destroy(host);
        enet_deinitialize();
    }
}
