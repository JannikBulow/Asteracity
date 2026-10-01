// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_COMPONENTS_VELOCITY_H
#define ASTERACITY_ENGINE_WORLD_COMPONENTS_VELOCITY_H

#include "engine/util/math.h"

namespace engine {
    struct Velocity {
        math::Vec2 linear = math::Vec2::Zero();
    };
}

#endif //ASTERACITY_ENGINE_WORLD_COMPONENTS_VELOCITY_H
