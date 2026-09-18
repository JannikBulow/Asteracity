// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_COMPONENT_H
#define ASTERACITY_ENGINE_WORLD_COMPONENT_H

#include "engine/util/object_allocator.h"

#include "engine/world/entity.h"

#include <memory>
#include <tuple>

namespace engine {
    class IComponentStorage {
    public:
        virtual ~IComponentStorage() = default;

        virtual size_t size() const = 0;
        virtual Entity entityAt(size_t index) const = 0;
        virtual void removeSilentFail(Entity entity) = 0;
    };

    template<class T>
    requires(std::is_move_constructible_v<T>)
    class ComponentStorage : public IComponentStorage {
    public:
        size_t size() const override { return mDense.size(); }

        Entity entityAt(size_t index) const override {
            return mDense[index].entity;
        }

        T& add(Entity entity, T value = {}) {
            if (contains(entity)) throw util::GameException();

            T* component = mComponentAllocator.create(std::move(value));
            uint32_t dense = mDense.size();
            mDense.emplace_back(entity, component);

            ensureSparseSize(entity.index);
            mSparse[entity.index] = dense;

            return *component;
        }

        void removeSilentFail(Entity entity) override {
            if (contains(entity)) removeImpl(entity);
        }

        void remove(Entity entity) {
            if (!contains(entity)) throw util::GameException();
            removeImpl(entity);
        }

        bool contains(Entity entity) const {
            if (entity.index >= mSparse.size()) return false;

            uint32_t dense = mSparse[entity.index];
            return dense != INVALID_DENSE && mDense[dense].entity == entity;
        }

        T* get(Entity entity) {
            if (!contains(entity)) return nullptr;
            return mDense[mSparse[entity.index]].component;
        }

    private:
        struct DenseEntity {
            Entity entity;
            T* component;
        };

        static constexpr uint32_t INVALID_DENSE = std::numeric_limits<uint32_t>::max();

        std::vector<DenseEntity> mDense;
        std::vector<uint32_t> mSparse;
        util::ObjectAllocator<T> mComponentAllocator;

        void ensureSparseSize(uint32_t index) {
            if (index >= mSparse.size()) mSparse.resize(static_cast<size_t>(index) + 1, INVALID_DENSE);
        }

        void removeImpl(Entity entity) {
            uint32_t removed = mSparse[entity.index];
            uint32_t last = mDense.size() - 1;

            DenseEntity moved = std::move(mDense[last]);

            T* component = mDense[removed].component;

            mDense[removed] = std::move(moved);
            mSparse[moved.entity.index] = removed;
            mDense.pop_back();

            mComponentAllocator.destroy(component);
            mSparse[entity.index] = INVALID_DENSE;
        }
    };

    template<class... Components>
    class ComponentView {
        friend class Iterator;
    public:
        class Iterator {
        public:
            Iterator(ComponentView& view, size_t index)
                : mView(view)
                , mIndex(index) {
                skipInvalid();
            }

            Iterator& operator++() {
                ++mIndex;
                skipInvalid();
                return *this;
            }

            bool operator!=(const Iterator& other) const {
                return mIndex != other.mIndex;
            }

            auto operator*() const {
                Entity entity = mView.baseEntity(mIndex);
                return std::tuple<Entity, Components&...>{
                    entity,
                    mView.template get<Components>(entity)...
                };
            }

        private:
            ComponentView& mView;
            size_t mIndex;

            void skipInvalid() {
                while (mIndex < mView.mSize) {
                    Entity entity = mView.baseEntity(mIndex);
                    if (mView.isEligible(entity)) return;
                    ++mIndex;
                }
            }
        };

        ComponentView(ComponentStorage<Components>&... storages)
            : mStorages(&storages...) {
            mBaseStorage = findSmallestStorage();
            if (!mBaseStorage) throw util::GameException();
            mSize = mBaseStorage->size();
        }

        bool isEligible(Entity entity) const {
            return std::apply([&](auto*... storage) {
                return (storage->contains(entity) && ...);
            }, mStorages);
        }

        Iterator begin() {
            return Iterator(*this, 0);
        }

        Iterator end() {
            return Iterator(*this, mSize);
        }

    private:
        std::tuple<ComponentStorage<Components>*...> mStorages;

        IComponentStorage* mBaseStorage = nullptr;
        size_t mSize = 0;

        IComponentStorage* findSmallestStorage() {
            size_t smallestSize = SIZE_MAX;

            IComponentStorage* smallestStorage = nullptr;

            std::apply([&](auto*... storage) {
                ([&] {
                    if (storage->size() < smallestSize) {
                        smallestSize = storage->size();
                        smallestStorage = storage;
                    }
                }(), ...);
            }, mStorages);

            return smallestStorage;
        }

        Entity baseEntity(size_t index) {
            return mBaseStorage->entityAt(index);
        }

        template<class T>
        T& get(Entity entity) {
            auto* storage = std::get<ComponentStorage<T>*>(mStorages);
            return *storage->get(entity);
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
        void remove(Entity entity) {
            storage<T>().remove(entity);
        }

        void onDestroyEntity(Entity entity) {
            for (auto& storage : mComponentStorages) {
                if (storage) storage->removeSilentFail(entity);
            }
        }

        template<class T>
        T* get(Entity entity) {
            return storage<T>().get(entity);
        }

        template<class... Components>
        ComponentView<Components...> view() {
            return ComponentView<Components...>(storage<Components>()...);
        }

    private:
        std::vector<std::unique_ptr<IComponentStorage>> mComponentStorages;
    };
}

#endif //ASTERACITY_ENGINE_WORLD_COMPONENT_H
