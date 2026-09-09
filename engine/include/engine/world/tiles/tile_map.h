// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_TILES_TILE_MAP_H
#define ASTERACITY_ENGINE_WORLD_TILES_TILE_MAP_H

#include "engine/world/tiles/tile.h"

#include <memory>

namespace engine {
    class TileMap {
    public:
        TileMap(int width, int height);

        int getWidth() const { return mWidth; }
        int getHeight() const { return mHeight; }

        Tile& get(TileCoord position);
        const Tile& get(TileCoord position) const;

    private:
        int mWidth;
        int mHeight;
        std::unique_ptr<Tile[]> mTiles;

        size_t index(int x, int y) const;
    };
}

#endif //ASTERACITY_ENGINE_WORLD_TILES_TILE_MAP_H
