// Copyright 2026 Jannik Laugmand Bülow

#include "asteracity/character/character_controller.h"
#include "asteracity/character/character_intent.h"
#include "asteracity/character/character_stats.h"

#include <engine/world/components/velocity.h>

#include <engine/engine.h>

namespace asteracity {
    void CharacterControllerSystem::update(engine::Engine& engine, float dt) {
        for (auto [entity, stats, intent, velocity] : engine.activeScene().world().viewEntities<CharacterStats, CharacterIntent, engine::Velocity>()) {
            velocity.linear = intent.movement * stats.movementSpeed;
        }
    }
}
