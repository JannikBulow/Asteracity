// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_RENDER_SPRITE_H
#define ASTERACITY_ENGINE_RENDER_SPRITE_H

#include "engine/resource/texture.h"

#include <optional>

namespace engine {
    class Sprite {
    public:
        explicit Sprite(Texture texture, std::optional<math::Rect> pixelBounds = std::nullopt, math::Color tint = math::Color::White);

        const Texture& texture() const { return mTexture; }
        math::Rect uv() const { return mUV; }
        math::Color tint() const { return mTint; }

    private:
        Texture mTexture;
        math::Rect mUV;
        math::Color mTint;
    };
}

#endif //ASTERACITY_ENGINE_RENDER_SPRITE_H
