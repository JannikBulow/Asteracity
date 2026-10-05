// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_GAME_H
#define ASTERACITY_GAME_H

#include <engine/engine.h>

namespace asteracity {
    class Game {
    public:
        explicit Game(engine::Engine& engine);

        engine::Engine& engine() const { return mEngine; }

        engine::Sprite getStaticColor(math::Color color) const;
        engine::TileID getRandomGrassTile() const;

        engine::AnimationClip& bushClip() { return mBushClip; }

        void registerSystems();
        void registerTiles();

    private:
        engine::Engine& mEngine;

        engine::Sprite mStaticColor; // 1x1 white sprite to be cloned

        engine::AnimationClip mBushClip;
    };
}

#endif //ASTERACITY_GAME_H
