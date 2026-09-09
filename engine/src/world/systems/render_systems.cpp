// Copyright 2026 Jannik Laugmand Bülow

#include "engine/world/components/renderer.h"
#include "engine/world/components/transform.h"

#include "engine/world/systems/render_systems.h"

#include "engine/engine.h"

namespace engine {
    void TileRenderSystem::render(Engine& engine) {
        Scene& activeScene = engine.activeScene();
        Camera& activeCamera = activeScene.activeCamera();

        math::Rect bounds = activeCamera.worldBounds(math::Vec2(engine.window().getFramebufferSize()));

        int leftTile = floor(bounds.left);
        int rightTile = ceil(bounds.right);
        int bottomTile = floor(bounds.bottom);
        int topTile = ceil(bounds.top);

        for (int y = bottomTile; y <= topTile; y++) {
            for (int x = leftTile; x <= rightTile; x++) {
                Tile tile = activeScene.tileWorld().get({x, y});
                const TileDefinition& definition = engine.tileRegistry()[tile.id];

                engine.renderer().drawSprite(definition.sprite, {static_cast<float>(x), static_cast<float>(y)}, math::Vec2::One());
            }
        }
    }

    void RenderSystem::render(Engine& engine) {
        Scene& activeScene = engine.activeScene();

        for (auto [entity, transform, renderer] : activeScene.world().viewEntities<Transform, SpriteRenderer>()) {
            engine.renderer().drawSprite(renderer.sprite, transform.position, transform.scale, transform.rotation);
        }
    }
}
