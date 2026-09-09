// Copyright 2026 Jannik Laugmand Bülow

#include "engine/world/tiles/tile_world.h"

namespace engine {
    TileWorld::TileWorld(TileRegistry& registry, int width, int height)
        : mRegistry(registry)
        , mMap(width, height) {}

    Tile& TileWorld::get(math::Vec2I worldPos) {
        return mMap.get(worldToTile(worldPos));
    }

    const Tile& TileWorld::get(math::Vec2I worldPos) const {
        return mMap.get(worldToTile(worldPos));
    }

    TileCoord TileWorld::worldToTile(math::Vec2I worldPos) const {
        return {
            worldPos.x + mMap.getWidth() / 2,
            mMap.getHeight() / 2 - worldPos.y
        };
    }
}
