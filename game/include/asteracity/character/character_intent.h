// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_CHARACTER_CHARACTER_INTENT_H
#define ASTERACITY_CHARACTER_CHARACTER_INTENT_H

#include <engine/util/math.h>

namespace asteracity {
    struct CharacterIntent {
        math::Vec2 movement = math::Vec2::Zero();
    };
}

#endif //ASTERACITY_CHARACTER_CHARACTER_INTENT_H
