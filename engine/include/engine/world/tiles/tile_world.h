// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_TILES_TILE_WORLD_H
#define ASTERACITY_ENGINE_WORLD_TILES_TILE_WORLD_H

#include "engine/world/tiles/registry.h"
#include "engine/world/tiles/tile_map.h"

namespace engine {
    class TileWorld {
    public:
        explicit TileWorld(TileRegistry& registry, int width, int height);

        const TileRegistry& registry() const { return mRegistry; }

        Tile& get(math::Vec2I worldPos);
        const Tile& get(math::Vec2I worldPos) const;

    private:
        TileRegistry& mRegistry;

        TileMap mMap;

        TileCoord worldToTile(math::Vec2I worldPos) const;
    };
}

#endif //ASTERACITY_ENGINE_WORLD_TILES_TILE_WORLD_H
