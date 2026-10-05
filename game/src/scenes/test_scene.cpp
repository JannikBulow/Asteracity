// Copyright 2026 Jannik Laugmand Bülow

#include "asteracity/scenes/test_scene.h"

#include "asteracity/tile/registry.h"

#include <engine/world/components/animator.h>
#include <engine/world/components/renderer.h>
#include <engine/world/components/transform.h>

namespace asteracity {
    std::unique_ptr<engine::Scene> CreateTestScene(Game& game) {
        auto scene = std::make_unique<engine::Scene>(game.engine().tileRegistry(), math::Vec2I{11, 11});

        int minX = -scene->tileWorld().getWidth() / 2;
        int maxX = scene->tileWorld().getWidth() / 2;
        int minY = -scene->tileWorld().getHeight() / 2;
        int maxY = scene->tileWorld().getHeight() / 2;

        for (int x = minX; x <= maxX; x++) {
            for (int y = minY; y <= maxY; y++) {
                engine::Tile& tile = scene->tileWorld().get({x, y});
                tile.id = game.getRandomGrassTile();

                if (x == minX || x == maxX || y == minY || y == maxY) {
                    engine::World& world = scene->world();
                    engine::Entity entity = world.createEntity();
                    world.addComponent<engine::Transform>(entity, engine::Transform{.position = {static_cast<float>(x), static_cast<float>(y)}});
                    world.addComponent<engine::SpriteAnimator>(entity, engine::Animation(game.bushClip()));
                    world.addComponent<engine::SpriteRenderer>(entity, game.bushClip().getFrames().front().sprite);
                }
            }
        }

        return scene;
    }
}
