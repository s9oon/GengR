#include "GengR/Engine/input.hpp"
#include <raylib.h>

namespace GengR::Engine {
    bool isKeyDown(Key key) {
        switch (key) {
            case Key::W: return IsKeyDown(KEY_W);
            case Key::A: return IsKeyDown(KEY_A);
            case Key::S: return IsKeyDown(KEY_S);
            case Key::D: return IsKeyDown(KEY_D);
        }
        return false;
    }
}