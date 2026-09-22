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
        if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) return static_cast<size_t>(mWidth * mHeight);
        return static_cast<size_t>(y) * mWidth + x;
    }
}
