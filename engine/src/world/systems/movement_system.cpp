// Copyright 2026 Jannik Laugmand Bülow

#include "engine/world/components/rigid_body.h"
#include "engine/world/components/transform.h"

#include "engine/world/systems/movement_system.h"

#include "engine/engine.h"

#include <iostream>

namespace engine {
    void MovementSystem::update(Engine& engine, float dt) {
        World& world = engine.activeScene().world();

        for (auto [entity, transform, rb] : world.viewEntities<Transform, RigidBody>()) {
            transform.position += rb.linearVelocity * dt;
        }
    }
}
