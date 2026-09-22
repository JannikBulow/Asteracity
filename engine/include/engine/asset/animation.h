// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_ASSET_ANIMATION_H
#define ASTERACITY_ENGINE_ASSET_ANIMATION_H

#include "engine/asset/sprite.h"

namespace engine {
    struct AnimationFrame {
        Sprite sprite;
        float duration;
    };

    class AnimationClip {
    public:
        explicit AnimationClip(std::vector<AnimationFrame> frames);

        std::span<const AnimationFrame> getFrames() const { return mFrames; }
        float getDuration() const { return mDuration; }

    private:
        std::vector<AnimationFrame> mFrames;
        float mDuration;
    };

    class Animation {
    public:
        explicit Animation(const AnimationClip& clip);

        bool isPlaying() const { return mPlaying; }
        const AnimationFrame& getCurrentFrame() const { return mClip->getFrames()[mCurrentFrameIndex]; }

        void play();
        void stop();
        void restart();
        void pause();
        void resume();

        void update(float dt);

    private:
        const AnimationClip* mClip;
        bool mPlaying = true;
        float mAccumulatedTime = 0.0f;
        size_t mCurrentFrameIndex = 0;
    };
}

#endif //ASTERACITY_ENGINE_ASSET_ANIMATION_H
