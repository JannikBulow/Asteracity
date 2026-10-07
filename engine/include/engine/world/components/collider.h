// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_COMPONENTS_COLLIDER_H
#define ASTERACITY_ENGINE_WORLD_COMPONENTS_COLLIDER_H

#include "engine/util/math.h"

#include <optional>

namespace engine {
    struct Collider {
        std::optional<math::Rect> bounds = std::nullopt;
        bool trigger = false;
    };
}

#endif //ASTERACITY_ENGINE_WORLD_COMPONENTS_COLLIDER_H
