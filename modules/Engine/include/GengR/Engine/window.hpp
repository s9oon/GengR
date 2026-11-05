#pragma once
#include "GengR/Net/types.hpp" 
#include "GengR/Engine/color.hpp"

namespace GengR::Engine {
    class Window {
    public:
        Window(const char* title, int width, int height); // throws on failure
        ~Window();

        float frameTime() const;

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        bool shouldClose() const;

        void beginFrame();
        void endFrame();

        void drawSquare(Vec2 pos, float size, Color color);
    };
}