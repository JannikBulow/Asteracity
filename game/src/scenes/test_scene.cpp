// Copyright 2026 Jannik Laugmand Bülow

#include "asteracity/scenes/test_scene.h"

namespace asteracity {
    std::unique_ptr<engine::Scene> CreateTestScene(Game& game) {
        auto scene = std::make_unique<engine::Scene>(game.engine().tileRegistry(), math::Vec2I{11, 11});

        for (int x = -5; x <= 5; x++) {
            for (int y = -5; y <= 5; y++) {
                scene->tileWorld().get({x, y}).id = game.getRandomGrassTile();
            }
        }

        return scene;
    }
}
