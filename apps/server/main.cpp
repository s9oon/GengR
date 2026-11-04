#include "GengR/Server/server.hpp"
#include <exception>
#include <print>

int main() {
    GengR::Server::Config config;

    try {
        GengR::Server::Host server(config);

        std::println("Server running on [{}]:{}", config.host, config.port);

        server.start(); // runs until stop() is called
    } catch (const std::exception& e) {
        std::println(stderr, "error: {}", e.what());
        return 1;
    }
}
