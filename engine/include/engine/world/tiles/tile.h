// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_TILES_TILE_H
#define ASTERACITY_ENGINE_WORLD_TILES_TILE_H

#include "engine/asset/animation.h"
#include "engine/asset/sprite.h"

#include <cstdint>

namespace engine {
    // not a Vec2I type alias because then the strict coordinate type wouldn't matter
    struct TileCoord {
        int x, y;
    };

    struct TileDefinition {
        Sprite sprite;
        std::optional<Animation> animation = std::nullopt;

        bool isAnimation() const { return animation.has_value(); }
    };

    using TileID = uint32_t;

    struct Tile {
        TileID id;
    };
}

#endif //ASTERACITY_ENGINE_WORLD_TILES_TILE_H
