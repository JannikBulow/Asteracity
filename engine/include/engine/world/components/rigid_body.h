// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_COMPONENTS_RIGID_BODY_H
#define ASTERACITY_ENGINE_WORLD_COMPONENTS_RIGID_BODY_H

#include "engine/util/math.h"

namespace engine {
    struct RigidBody {
        math::Vec2 linearVelocity = math::Vec2::Zero();
        float angularVelocity = 0.0f;

        float mass;
        float inverseMass;
        float restitution;
        float friction;

        RigidBody(float mass, float restitution, float friction) : mass(mass), inverseMass(1.0f / mass), restitution(restitution), friction(friction) {}
    };
}

#endif //ASTERACITY_ENGINE_WORLD_COMPONENTS_RIGID_BODY_H
