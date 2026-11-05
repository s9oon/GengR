#include "GengR/Engine/window.hpp"
#include <raylib.h>
#include <stdexcept>

namespace {
    // GengR types -> raylib types. Only this file sees raylib.
    // ::Color / ::Vector2 are raylib's (global scope); plain Color inside GengR::Engine is ours.
    ::Vector2 toRaylib(GengR::Vec2 v) { return { v.x, v.y }; }
    ::Color toRaylib(GengR::Engine::Color c) { return { c.r, c.g, c.b, c.a }; }
}

namespace GengR::Engine {
    Window::Window(const char* title, int width, int height) {
        InitWindow(width, height, title);

        // InitWindow returns nothing, so ask raylib whether it worked
        if (!IsWindowReady()) {
            throw std::runtime_error("couldn't create window");
        }

        SetTargetFPS(60);
    }

    // runs automatically when the Window goes out of scope
    Window::~Window() {
        CloseWindow();
    }

    bool Window::shouldClose() const {
        return WindowShouldClose(); // true on the close button or Esc
    }

    void Window::beginFrame() {
        BeginDrawing();
        ClearBackground(toRaylib(Colors::Black));
    }

    void Window::endFrame() {
        EndDrawing();
    }

    void Window::drawSquare(Vec2 pos, float size, Color color) {
        DrawRectangleV(toRaylib(pos), { size, size }, toRaylib(color));
    }
}
