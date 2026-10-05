// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_SCENES_TEST_SCENE_H
#define ASTERACITY_SCENES_TEST_SCENE_H

#include "asteracity/game.h"

namespace asteracity {
    std::unique_ptr<engine::Scene> CreateTestScene(Game& game);
}

#endif //ASTERACITY_SCENES_TEST_SCENE_H
