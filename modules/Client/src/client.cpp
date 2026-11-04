#include "GengR/Client/client.hpp"

namespace GengR::Client {
    Host::Host(const Config& config)
        : host(std::nullopt, // no bind address: a client only connects out
               1,            // only 1 outgoing connection, to the server
               config.maxChannels,
               config.incomingBandwidth,
               config.outgoingBandwidth) {
    }
}
