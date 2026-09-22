// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_ASSET_ASSET_MANAGER_H
#define ASTERACITY_ENGINE_ASSET_ASSET_MANAGER_H

#include "engine/asset/animation.h"
#include "engine/asset/sprite.h"

#include "engine/resource/resource_manager.h"

namespace engine {
    // assets are things of data that don't require any lifetime management and are usually lightweight
    // assets usually rely on some sort of resource under the hood
    class AssetManager {
    public:
        explicit AssetManager(ResourceManager& resourceManager);

        Sprite loadSprite(const util::ResourceLocation& location);
        AnimationClip loadAnimation(const util::ResourceLocation& location);

    private:
        ResourceManager& mResourceManager;
    };
}

#endif //ASTERACITY_ENGINE_ASSET_ASSET_MANAGER_H
