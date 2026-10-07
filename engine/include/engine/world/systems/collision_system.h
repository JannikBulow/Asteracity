// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_SYSTEMS_COLLISION_SYSTEM_H
#define ASTERACITY_ENGINE_WORLD_SYSTEMS_COLLISION_SYSTEM_H

#include "engine/util/math.h"

#include "engine/world/entity.h"
#include "engine/world/system.h"

namespace engine {
    struct RigidBody;
    struct Transform;

    class CollisionSystem : public ISystem {
    public:
        void update(Engine& engine, float dt) override;

    private:
        struct Collision {
            Entity a;
            Entity b;

            math::Vec2 normal;
            float penetration;
            math::Vec2 contactPoint;
        };

        void correctDynamicCollision(const Collision& collision, Transform& transformA, Transform& transformB, RigidBody& rbA, RigidBody& rbB);
    };
}

#endif //ASTERACITY_ENGINE_WORLD_SYSTEMS_COLLISION_SYSTEM_H
