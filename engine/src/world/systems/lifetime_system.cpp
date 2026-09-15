// Copyright 2026 Jannik Laugmand Bülow

#include "engine/world/components/lifetime.h"

#include "engine/world/systems/lifetime_system.h"

#include "engine/engine.h"

namespace engine {
    void LifetimeSystem::update(Engine& engine, float dt) {
        World& world = engine.activeScene().world();
        std::vector<Entity> deadEntities;

        for (auto [entity, lifetime] : world.viewEntities<LifetimeComponent>()) {
            lifetime.remainingSeconds -= dt;
            if (lifetime.remainingSeconds <= 0) {
                deadEntities.push_back(entity);
            }
        }

        for (auto entity : deadEntities) {
            world.destroyEntity(entity);
        }
    }
}
