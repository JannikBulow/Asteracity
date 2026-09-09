// Copyright 2026 Jannik Laugmand Bülow

#include "engine/world/tiles/registry.h"

namespace engine {
    void TileRegistry::registerTile(TileID assignedID, TileDefinition definition) {
        ensureDefinitionsSize(assignedID);

        if (mDefinitions[assignedID].has_value()) throw util::GameException();

        mDefinitions[assignedID] = std::move(definition);
    }

    const TileDefinition& TileRegistry::get(TileID id) const {
        if (!mDefinitions[id].has_value()) throw util::GameException();
        return *mDefinitions[id];
    }

    void TileRegistry::ensureDefinitionsSize(TileID assignedID) {
        if (assignedID >= mDefinitions.size()) mDefinitions.resize(static_cast<size_t>(assignedID) + 1, std::nullopt);
    }
}
