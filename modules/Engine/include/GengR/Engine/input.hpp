// modules/Engine/include/GengR/Engine/input.hpp
#pragma once

namespace GengR::Engine {
    enum class Key { W, A, S, D };

    bool isKeyDown(Key key);   // true every frame the key is held
}