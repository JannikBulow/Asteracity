// Copyright 2026 Jannik Laugmand Bülow

#include "engine/asset/asset_manager.h"

#include <file_formats/sprite.h>

namespace engine {
    AssetManager::AssetManager(ResourceManager& resourceManager)
        : mResourceManager(resourceManager) {}

    Sprite AssetManager::loadSprite(const util::ResourceLocation& location) {
        formats::BinaryReader<formats::FileInput> reader(fopen(location.cstr(), "rb"));
        formats::Sprite spriteData = reader.read<formats::Sprite>();

        Texture texture = mResourceManager.createTexture(spriteData.texture);

        return Sprite{
            texture,
            {spriteData.size.width, spriteData.size.height},
            spriteData.uv.has_value() ? std::optional(math::Rect(spriteData.uv->left, spriteData.uv->right, spriteData.uv->top, spriteData.uv->bottom)) : std::nullopt,
            {spriteData.tint.r, spriteData.tint.g, spriteData.tint.b, spriteData.tint.a},
        };
    }
}
