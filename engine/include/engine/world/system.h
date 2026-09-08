// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_SYSTEM_H
#define ASTERACITY_ENGINE_WORLD_SYSTEM_H

namespace engine {
    class Engine;

    class ISystem {
    public:
        virtual ~ISystem() = default;

        virtual void preUpdate(Engine& engine, float dt) {}
        virtual void update(Engine& engine, float dt) {}
        virtual void postUpdate(Engine& engine, float dt) {}
        virtual void render(Engine& engine) {} // world render!
        virtual void renderUI(Engine& engine) {}
    };
}

#endif //ASTERACITY_ENGINE_WORLD_SYSTEM_H
