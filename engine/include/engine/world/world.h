// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_WORLD_H
#define ASTERACITY_ENGINE_WORLD_WORLD_H

#include "engine/world/component.h"

namespace engine {
    class World {
    public:
        Entity createEntity();
        void destroyEntity(Entity entity);

        template<class T>
        T& addComponent(Entity entity, T component = {}) {
            if (std::is_move_constructible_v<T>) return mComponentManager.add<T>(entity, std::move(component));
            else return mComponentManager.add<T>(entity, component);
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
