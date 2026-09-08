// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_ENTITY_H
#define ASTERACITY_ENGINE_WORLD_ENTITY_H

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

#endif //ASTERACITY_ENGINE_WORLD_ENTITY_H
