// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_RENDER_SPRITE_H
#define ASTERACITY_ENGINE_RENDER_SPRITE_H

#include "engine/resource/texture.h"

#include <optional>

namespace engine {
    class Sprite {
    public:
        explicit Sprite(Texture texture, std::optional<math::Rect> pixelBounds = std::nullopt);

        const Texture& texture() const { return mTexture; }
        math::Rect uv() const { return mUV; }

    private:
        Texture mTexture;
        math::Rect mUV;
    };
}

#endif //ASTERACITY_ENGINE_RENDER_SPRITE_H
