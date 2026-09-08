// Copyright 2026 Jannik Laugmand Bülow

#include "engine/world/components/renderer.h"
#include "engine/world/components/transform.h"

#include "engine/world/systems/render_system.h"

#include "engine/engine.h"

namespace engine {
    void RenderSystem::render(Engine& engine) {
        Scene& activeScene = engine.activeScene();

        for (auto [entity, transform, renderer] : activeScene.world().viewEntities<Transform, SpriteRenderer>()) {
            engine.renderer().drawSprite(renderer.sprite, transform.position, transform.scale, transform.rotation);
        }
    }
}
