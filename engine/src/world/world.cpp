// Copyright 2026 Jannik Laugmand Bülow

#include "engine/world/world.h"

namespace engine {
    Entity World::createEntity() {
        return mEntityManager.create();
    }

    void World::destroyEntity(Entity entity) {
        mComponentManager.onDestroyEntity(entity);
        mEntityManager.destroy(entity);
    }
}
