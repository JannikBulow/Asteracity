// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_TILES_REGISTRY_H
#define ASTERACITY_ENGINE_WORLD_TILES_REGISTRY_H

#include "engine/world/tiles/tile.h"

namespace engine {
    class TileRegistry {
    public:
        void registerTile(TileID assignedID, TileDefinition definition);

        // safe access
        const TileDefinition& get(TileID id) const;

        // fast access
        const TileDefinition& operator[](TileID id) const {
            return *mDefinitions[id];
        }

    private:
        std::vector<std::optional<TileDefinition>> mDefinitions;

        void ensureDefinitionsSize(TileID assignedID);
    };
}

#endif //ASTERACITY_ENGINE_WORLD_TILES_REGISTRY_H
