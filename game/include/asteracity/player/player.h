// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_PLAYER_PLAYER_H
#define ASTERACITY_PLAYER_PLAYER_H

#include <engine/engine.h>

namespace asteracity {
    class Game;

    class Player {
    public:
        Player(Game& game, engine::Scene& scene);
        ~Player();

    private:
        engine::Scene& mScene;
        engine::AnimationClip mAnimation;
        engine::Entity mEntity;
    };
}

#endif //ASTERACITY_PLAYER_PLAYER_H
