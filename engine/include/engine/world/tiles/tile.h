// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_TILES_TILE_H
#define ASTERACITY_ENGINE_WORLD_TILES_TILE_H

#include "engine/render/sprite.h"

#include <cstdint>

namespace engine {
    // not a Vec2I type alias because then the strict coordinate type wouldn't matter
    struct TileCoord {
        int x, y;
    };

    struct TileDefinition {
        Sprite sprite;
    };

    using TileID = uint32_t;

    struct Tile {
        TileID id;
    };
}

#endif //ASTERACITY_ENGINE_WORLD_TILES_TILE_H
