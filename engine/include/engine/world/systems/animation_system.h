// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_SYSTEMS_ANIMATION_SYSTEM_H
#define ASTERACITY_ENGINE_WORLD_SYSTEMS_ANIMATION_SYSTEM_H

#include "engine/world/system.h"

namespace engine {
    class AnimationSystem : public ISystem {
    public:
        void update(Engine& engine, float dt) override;
    };
}

#endif //ASTERACITY_ENGINE_WORLD_SYSTEMS_ANIMATION_SYSTEM_H
