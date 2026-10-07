// Copyright 2026 Jannik Laugmand Bülow

#include "asteracity/character/character_controller.h"
#include "asteracity/character/character_intent.h"
#include "asteracity/character/character_stats.h"

#include <engine/world/components/rigid_body.h>

#include <engine/engine.h>

namespace asteracity {
    void CharacterControllerSystem::update(engine::Engine& engine, float dt) {
        for (auto [entity, stats, intent, rb] : engine.activeScene().world().viewEntities<CharacterStats, CharacterIntent, engine::RigidBody>()) {
            rb.linearVelocity = intent.movement * stats.movementSpeed;
        }
    }
}
