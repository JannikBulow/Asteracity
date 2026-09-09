// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_SYSTEMS_RENDER_SYSTEM_H
#define ASTERACITY_ENGINE_WORLD_SYSTEMS_RENDER_SYSTEM_H

#include "engine/world/system.h"

namespace engine {
    class TileRenderSystem : public ISystem {
    public:
        void render(Engine& engine) override;
    };

    class RenderSystem : public ISystem {
    public:
        void render(Engine& engine) override;
    };
}

#endif //ASTERACITY_ENGINE_WORLD_SYSTEMS_RENDER_SYSTEM_H
