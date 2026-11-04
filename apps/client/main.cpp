#include "GengR/Client/client.hpp"
#include <exception>
#include <print>

int main() {
    GengR::Client::Config config;

    try {
        GengR::Client::Host client(config);

        std::println("Client host created");
    } catch (const std::exception& e) {
        std::println(stderr, "error: {}", e.what());
        return 1;
    }
}
