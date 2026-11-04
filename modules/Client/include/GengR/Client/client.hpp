#pragma once

#include <cstddef>
#include <cstdint>
#include "GengR/Net/host.hpp"

namespace GengR::Client {

    struct Config {
        std::size_t maxChannels = 2; // must match the server's

        std::uint32_t incomingBandwidth = 0;
        std::uint32_t outgoingBandwidth = 0;
    };

    class Host {
    public:
        explicit Host(const Config& config); // throws std::runtime_error on failure

    private:
        Net::Host host; // cleans itself up, so Client::Host needs no destructor
    };

}
