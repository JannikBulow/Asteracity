// Copyright 2026 Jannik Laugmand Bülow

#include "engine/asset/animation.h"

namespace engine {
    AnimationClip::AnimationClip(std::vector<AnimationFrame> frames)
        : mFrames(std::move(frames))
        , mDuration(0) {
        for (const auto& frame : mFrames) {
            mDuration += frame.duration;
        }
    }

    Animation::Animation(const AnimationClip& clip)
        : mClip(&clip) {}

    void Animation::play() {
        if (mPlaying) return;

        mPlaying = true;
        mAccumulatedTime = 0.0f;
        mCurrentFrameIndex = 0;
    }

    void Animation::stop() {
        mPlaying = false;
        mAccumulatedTime = 0.0f;
        mCurrentFrameIndex = 0;
    }

    void Animation::restart() {
        mPlaying = true;
        mAccumulatedTime = 0.0f;
        mCurrentFrameIndex = 0;
    }

    void Animation::pause() {
        mPlaying = false;
        mAccumulatedTime = 0.0f;
    }

    void Animation::resume() {
        mPlaying = true;
    }

    void Animation::update(float dt) {
        if (!mPlaying) return;

        mAccumulatedTime += dt;
        if (mAccumulatedTime >= getCurrentFrame().duration) {
            mAccumulatedTime = 0.0f;
            mCurrentFrameIndex += 1;
            if (mCurrentFrameIndex >= mClip->getFrames().size()) mCurrentFrameIndex = 0;
        }
    }
}
