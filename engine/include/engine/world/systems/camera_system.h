// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_SYSTEMS_CAMERA_SYSTEM_H
#define ASTERACITY_ENGINE_WORLD_SYSTEMS_CAMERA_SYSTEM_H

#include "engine/world/system.h"

namespace engine {
    class CameraSystem : public ISystem {
    public:
        void postUpdate(Engine& engine, float dt) override;
    };
}

#endif //ASTERACITY_ENGINE_WORLD_SYSTEMS_CAMERA_SYSTEM_H
