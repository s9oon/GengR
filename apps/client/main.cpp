#include "GengR/Client/client.hpp"
#include <atomic>
#include <exception>
#include <iostream>
#include <mutex>
#include <print>
#include <queue>
#include <string>
#include <thread>
#include "GengR/Engine/input.hpp"
#include "GengR/Engine/window.hpp"

int main() {
    using namespace GengR::Engine;
    using GengR::Vec2;

    try {
        GengR::Client::Config config;
        GengR::Client::Host client(config);

        // only starts connecting; pollEvents() in the loop finishes it, so the window opens straight away
        // until it's connected, sendPosition does nothing and you just move alone
        client.connectToServer({ "::1", 1234 }); // the server on this machine

        Window window("GengR", 800, 450); // opens the window; the destructor closes it

        Vec2 pos { 375, 200 };        // top-left corner of the square
        const float speed = 300.0f;   // pixels per second

        while (!window.shouldClose()) {
            client.pollEvents(0); // 0 = don't wait; runs onPlayerPosition for anything that arrived

            float dt = window.frameTime(); // seconds since the last frame

            // -1, 0 or +1 on each axis; screen y grows downwards, so up is minus
            Vec2 dir {};
            if (isKeyDown(Key::W)) dir.y -= 1;
            if (isKeyDown(Key::S)) dir.y += 1;
            if (isKeyDown(Key::A)) dir.x -= 1;
            if (isKeyDown(Key::D)) dir.x += 1;

            pos += dir * (speed * dt);

            client.sendPosition(pos); // tell the server where we are; it forwards it to everyone else

            window.beginFrame();
            window.drawSquare(pos, 50, Colors::Red); // us
            for (const auto& [id, p] : client.otherPlayers()) {
                window.drawSquare(p, 50, Colors::Blue); // everyone else
            }
            window.endFrame();
        }
    } catch (const std::exception& e) {
        std::println(stderr, "error: {}", e.what());
        return 1;
    }
}

/*
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

        std::println("Connected to server. Type a message and press Enter (Ctrl+Z then Enter to quit).");

        // input thread: getline blocks until Enter, so it gets its own thread
        std::thread inputThread([] {
            std::string line;
            while (running && std::getline(std::cin, line)) {
                std::lock_guard lock(inputMutex); // only one thread touches the queue at a time
                inputLines.push(line);
            }
            running = false; // end of input: quit
        });

        // main thread: keeps polling so messages arrive and the connection stays alive
        while (running && client.isConnected()) {
            client.pollEvents(10);

            std::lock_guard lock(inputMutex);
            while (!inputLines.empty()) {
                client.sendMessage(inputLines.front());
                inputLines.pop();
            }
        }

        // flush anything just sent before exiting
        for (int i = 0; i < 10; ++i) {
            client.pollEvents(10);
        }

        if (!client.isConnected()) {
            std::println("Disconnected from server");
        }

        running = false;
        inputThread.detach(); // getline may still be waiting; let it go when the program ends
    } catch (const std::exception& e) {
        std::println(stderr, "error: {}", e.what());
        return 1;
    }
}
*/