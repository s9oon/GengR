#include "GengR/Client/client.hpp"
#include <exception>
#include <print>

int main() {
    GengR::Client::Config config;

    try {
        GengR::Client::Host client(config);

        client.connectToServer({ "::1", 1234 }); // the server on this machine

        // give the server up to 5 seconds (500 x 10ms) to answer
        for (int i = 0; i < 500 && !client.isConnected(); ++i) {
            client.pollEvents(10);
        }

        if (!client.isConnected()) {
            std::println(stderr, "error: couldn't connect to the server");
            return 1;
        }

        std::println("Connected to server");

        client.sendMessage("hello");

        // sendMessage only queues the packet; polling gives ENet the chance to actually send it
        for (int i = 0; i < 10; ++i) {
            client.pollEvents(10);
        }
    } catch (const std::exception& e) {
        std::println(stderr, "error: {}", e.what());
        return 1;
    }
}
