// Copyright 2026 Jannik Laugmand Bülow

#include "engine/world/tiles/tile_map.h"

#include <algorithm>

namespace engine {
    TileMap::TileMap(int width, int height)
        : mWidth(width)
        , mHeight(height)
        , mTiles(std::make_unique<Tile[]>(mWidth * mHeight + 1)) {
        std::fill_n(mTiles.get(), mWidth * mHeight + 1, Tile(0));
    }

    Tile& TileMap::get(TileCoord position) {
        return mTiles[index(position.x, position.y)];
    }

    const Tile& TileMap::get(TileCoord position) const {
        return mTiles[index(position.x, position.y)];
    }

    size_t TileMap::index(int x, int y) const {
        size_t index = y * mWidth + x;
        if (index >= mWidth * mHeight) return mWidth * mHeight; // secret void tile!!! always id 0
        return index;
    }
}
