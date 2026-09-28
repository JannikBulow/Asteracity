// Copyright 2026 Jannik Laugmand Bülow

#include "engine/world/components/transform.h"
#include "engine/world/components/velocity.h"

#include "engine/world/systems/movement_system.h"

#include "engine/engine.h"

namespace engine {
    void MovementSystem::update(Engine& engine, float dt) {
        World& world = engine.activeScene().world();

        for (auto [entity, transform, velocity] : world.viewEntities<Transform, Velocity>()) {
            transform.position += velocity.linear * dt;
        }
    }
}
