// Copyright 2026 Jannik Laugmand Bülow

#include "engine/world/components/animator.h"

#include "engine/world/systems/animation_system.h"
#include "engine/world/components/renderer.h"

#include "engine/engine.h"

namespace engine {
    void AnimationSystem::update(Engine& engine, float dt) {
        World& world = engine.activeScene().world();

        for (auto [entity, animator] : world.viewEntities<SpriteAnimator>()) {
            animator.animation.update(dt);

            if (auto* renderer = world.getComponent<SpriteRenderer>(entity)) {
                const Sprite& sprite = animator.animation.getCurrentFrame().sprite;
                if (renderer->sprite != sprite) {
                    renderer->sprite = animator.animation.getCurrentFrame().sprite;
                }
            }
        }
    }
}
