// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_COMPONENTS_TRANSFORM_H
#define ASTERACITY_ENGINE_WORLD_COMPONENTS_TRANSFORM_H

#include "engine/util/math.h"

namespace engine {
    struct Transform {
        math::Vec2 position = math::Vec2::Zero();
        math::Vec2 scale = math::Vec2::One();
        float rotation = 0.0f;
    };
}

#endif //ASTERACITY_ENGINE_WORLD_COMPONENTS_TRANSFORM_H
