// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_WORLD_H
#define ASTERACITY_ENGINE_WORLD_WORLD_H

#include "engine/world/component.h"

namespace engine {
    class World {
    public:
        Entity createEntity();
        void destroyEntity(Entity entity);

        template<class T, class... Args>
        T& addComponent(Entity entity, Args&&... args) {
            return mComponentManager.add<T>(entity, T(std::forward<Args>(args)...));
        }

        template<class T>
        void removeComponent(Entity entity) {
            mComponentManager.remove<T>(entity);
        }

        template<class T>
        T* getComponent(Entity entity) {
            return mComponentManager.get<T>(entity);
        }

        template<class... Components>
        ComponentView<Components...> viewEntities() {
            return mComponentManager.view<Components...>();
        }

    private:
        EntityManager mEntityManager;
        ComponentManager mComponentManager;
    };
}

#endif //ASTERACITY_ENGINE_WORLD_WORLD_H
