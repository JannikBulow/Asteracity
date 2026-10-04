// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_CHARACTER_CHARACTER_CONTROLLER_H
#define ASTERACITY_CHARACTER_CHARACTER_CONTROLLER_H

#include <engine/world/system.h>

namespace asteracity {
    class CharacterControllerSystem : public engine::ISystem {
    public:
        void update(engine::Engine& engine, float dt) override;
    };
}

#endif //ASTERACITY_CHARACTER_CHARACTER_CONTROLLER_H
