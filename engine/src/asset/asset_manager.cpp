// Copyright 2026 Jannik Laugmand Bülow

#include "engine/asset/asset_manager.h"

#include <file_formats/animation.h>
#include <file_formats/sprite.h>

namespace engine {
    AssetManager::AssetManager(ResourceManager& resourceManager)
        : mResourceManager(resourceManager) {}

    Sprite AssetManager::loadSprite(const util::ResourceLocation& location) {
        formats::BinaryReader<formats::FileInput> reader(fopen(location.cstr(), "rb"));
        formats::Sprite spriteData = reader.read<formats::Sprite>();

        Texture texture = mResourceManager.createTexture(spriteData.texture, SamplerDescriptor(backend::TextureFilter::Nearest, backend::TextureWrap::Clamp));

        return Sprite{
            texture,
            {spriteData.size.width, spriteData.size.height},
            spriteData.uv.has_value() ? std::optional(math::Rect(spriteData.uv->left, spriteData.uv->right, spriteData.uv->top, spriteData.uv->bottom)) : std::nullopt,
            {spriteData.tint.r, spriteData.tint.g, spriteData.tint.b, spriteData.tint.a},
        };
    }

    AnimationClip AssetManager::loadAnimation(const util::ResourceLocation& location) {
        formats::BinaryReader<formats::FileInput> reader(fopen(location.cstr(), "rb"));
        formats::Animation animationData = reader.read<formats::Animation>();

        std::vector<AnimationFrame> frames;
        switch (animationData.type) {
            case formats::Animation::Type::FrameByFrame:
                frames.reserve(animationData.frames.size());
                for (const auto& frame : animationData.frames) {
                    frames.emplace_back(loadSprite(frame.sprite), frame.duration);
                }
                break;
            case formats::Animation::Type::Generated:
                Texture texture = mResourceManager.createTexture(animationData.texture, SamplerDescriptor(backend::TextureFilter::Nearest, backend::TextureWrap::Clamp));
                float frameWidth = static_cast<float>(texture.image().width) / static_cast<float>(animationData.columns);
                float frameHeight = static_cast<float>(texture.image().height) / static_cast<float>(animationData.rows);

                frames.reserve(animationData.rows * animationData.columns);

                for (uint32_t row = 0; row < animationData.rows; row++) {
                    for (uint32_t column = 0; column < animationData.columns; column++) {
                        float left = static_cast<float>(column) * frameWidth;
                        float right = left + frameWidth;
                        float top = static_cast<float>(row) * frameHeight;
                        float bottom = top + frameHeight;

                        frames.emplace_back(Sprite(
                            texture,
                        {animationData.frameSize.width, animationData.frameSize.height},
                        math::Rect(left, right, top, bottom)
                        ), animationData.frameDuration);
                    }
                }
                break;
        }

        return AnimationClip(std::move(frames));
    }
}
