// Copyright 2026 Jannik Laugmand Bülow

#ifndef UNNAMEDGAME_ENGINE_WORLD_ENTITY_H
#define UNNAMEDGAME_ENGINE_WORLD_ENTITY_H

#include "engine/util/handle.h"

namespace engine {
    using Entity = util::Handle<struct EntityTag>;

    class EntityManager {
    public:
        Entity create() { return mStorage.create(); }
        void destroy(Entity entity) { mStorage.destroy(entity); }
        bool isAlive(Entity entity) const { return mStorage.exists(entity); }

    private:
        util::HandleStorage<void, EntityTag> mStorage;
    };
}

#endif //UNNAMEDGAME_ENGINE_WORLD_ENTITY_H
