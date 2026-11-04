#include "GengR/Client/client.hpp"
#include <atomic>
#include <exception>
#include <iostream>
#include <mutex>
#include <print>
#include <queue>
#include <string>
#include <thread>

#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

namespace {
    // shared between the input thread and the main loop; at file scope so they outlive
    // the detached input thread, which may still be waiting in getline when main returns
    std::mutex inputMutex;
    std::queue<std::string> inputLines; // lines typed, waiting to be sent
    std::atomic<bool> running = true;
}

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("Hello World", 800, 600, SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
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