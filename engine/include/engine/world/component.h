// Copyright 2026 Jannik Laugmand Bülow

#ifndef UNNAMEDGAME_ENGINE_WORLD_COMPONENT_H
#define UNNAMEDGAME_ENGINE_WORLD_COMPONENT_H

#include "engine/world/entity.h"

#include <memory>

namespace engine {
    class IComponentStorage {
    public:
        virtual ~IComponentStorage() = default;

        virtual size_t size() const = 0;
        virtual Entity entityAt(size_t index) const = 0;
    };

    template<class T>
    requires(std::is_move_constructible_v<T>)
    class ComponentStorage : public IComponentStorage {
    public:
        size_t size() const override { return mEntities.size(); }

        T& add(Entity entity, T component = {}) {
            if (contains(entity)) throw util::GameException();

            uint32_t dense = mEntities.size();

            mEntities.push_back(entity);
            mComponents.push_back(std::move(component));

            ensureSparseSize(entity.index);

            mSparse[entity.index] = dense;

            return mComponents.back();
        }

        void remove(Entity entity) {
            if (!contains(entity)) throw util::GameException();

            uint32_t removed = mSparse[entity.index];
            uint32_t last = mEntities.size() - 1;

            Entity movedEntity = mEntities[last];

            mEntities[removed] = movedEntity;
            mComponents[removed] = std::move(mComponents[last]);

            mSparse[movedEntity.index] = removed;

            mEntities.pop_back();
            mComponents.pop_back();

            mSparse[entity.index] = INVALID_DENSE;
        }

        bool contains(Entity entity) const {
            if (entity.index >= mSparse.size()) return false;

            uint32_t dense = mSparse[entity.index];
            return dense != INVALID_DENSE && mEntities[dense] == entity;
        }

        T* get(Entity entity) {
            if (!contains(entity)) return nullptr;
            return &mComponents[mSparse[entity.index]];
        }

        Entity entityAt(size_t index) const override {
            return mEntities[index];
        }

    private:
        static constexpr uint32_t INVALID_DENSE = std::numeric_limits<uint32_t>::max();

        std::vector<Entity> mEntities;
        std::vector<T> mComponents;
        std::vector<uint32_t> mSparse;

        void ensureSparseSize(uint32_t index) {
            if (index >= mSparse.size()) mSparse.resize(static_cast<size_t>(index) + 1, INVALID_DENSE);
        }
    };

    using ComponentID = uint32_t;

    namespace internal {
        inline ComponentID nextComponentID = 0;
    }

    template<class T>
    ComponentID GetComponentID() {
        static ComponentID id = internal::nextComponentID++;
        return id;
    }

    class ComponentManager {
    public:
        template<class T>
        ComponentStorage<T>& storage() {
            ComponentID id = GetComponentID<T>();

            if (id < mComponentStorages.size() && mComponentStorages[id] != nullptr) return static_cast<ComponentStorage<T>&>(*mComponentStorages[id]);

            auto storage = std::make_unique<ComponentStorage<T>>();
            auto* ptr = storage.get();

            if (id >= mComponentStorages.size()) mComponentStorages.resize(static_cast<size_t>(id) + 1);
            mComponentStorages[id] = std::move(storage);

            return *ptr;
        }

        template<class T>
        T& add(Entity entity, T component = {}) {
            if (std::is_move_constructible_v<T>) return storage<T>().add(entity, std::move(component));
            else return storage<T>().add(entity, component);
        }

        template<class T>
        T* get(Entity entity) {
            return storage<T>().get(entity);
        }

    private:
        std::vector<std::unique_ptr<IComponentStorage>> mComponentStorages;
    };
}

#endif //UNNAMEDGAME_ENGINE_WORLD_COMPONENT_H
