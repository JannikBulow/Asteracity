// Copyright 2026 Jannik Laugmand Bülow

#include "engine/world/components/collider.h"
#include "engine/world/components/rigid_body.h"
#include "engine/world/components/transform.h"

#include "engine/world/systems/collision_system.h"

#include "engine/engine.h"

namespace engine {
    void CollisionSystem::update(Engine& engine, float dt) {
        auto colliders = engine.activeWorld().viewEntities<Transform, Collider>();
        for (auto a = colliders.begin(); a != colliders.end(); ++a) {
            auto b = a;
            ++b;

            for (; b != colliders.end(); ++b) {
                auto [entityA, transformA, colliderA] = *a;
                auto [entityB, transformB, colliderB] = *b;

                math::Rect rectA = !colliderA.bounds.has_value() ? math::Rect(transformA.position, transformA.scale) : colliderA.bounds->translate(transformA.position);
                math::Rect rectB = !colliderB.bounds.has_value() ? math::Rect(transformB.position, transformB.scale) : colliderB.bounds->translate(transformB.position);

                math::Vec2 overlap = rectA.getOverlap(rectB);

                if (overlap.x <= 0.0f || overlap.y <= 0.0f) continue;

                math::Vec2 delta = rectB.center() - rectA.center();

                math::Vec2 normal;
                float penetration;
                math::Vec2 contactPoint;
                if (overlap.x < overlap.y) {
                    normal = {delta.x < 0.0f ? -1.0f : 1.0f, 0.0f};
                    penetration = overlap.x;

                    float contactBottom = std::max(rectA.bottom, rectB.bottom);
                    float contactTop = std::min(rectA.top, rectB.top);
                    float contactX = normal.x > 0.0f ? rectA.right : rectA.left;
                    float contactY = (contactBottom + contactTop) * 0.5f;
                    contactPoint = {contactX, contactY};
                } else {
                    normal = {0.0f, delta.y < 0.0f ? -1.0f : 1.0f};
                    penetration = overlap.y;

                    float contactLeft = std::max(rectA.left, rectB.left);
                    float contactRight = std::min(rectA.right, rectB.right);
                    float contactX = (contactLeft + contactRight) * 0.5f;
                    float contactY = normal.y > 0.0f ? rectA.top : rectA.bottom;
                    contactPoint = {contactX, contactY};
                }

                Collision collision = {entityA, entityB, normal, penetration, contactPoint};

                RigidBody* rbA = engine.activeWorld().getComponent<RigidBody>(entityA);
                RigidBody* rbB = engine.activeWorld().getComponent<RigidBody>(entityB);

                if (rbA && !rbB) {
                    transformA.position -= collision.normal * collision.penetration;
                } else if (!rbA && rbB) {
                    transformB.position += collision.normal * collision.penetration;
                } else if (rbA && rbB) {
                    correctDynamicCollision(collision, transformA, transformB, *rbA, *rbB);
                }
            }
        }
    }

    void CollisionSystem::correctDynamicCollision(const Collision& collision, Transform& transformA, Transform& transformB, RigidBody& rbA, RigidBody& rbB) {
        float totalInverseMass = rbA.inverseMass + rbB.inverseMass;

        if (totalInverseMass > 0.0f) {
            math::Vec2 correction = collision.normal * (collision.penetration / totalInverseMass);

            transformA.position -= correction * rbA.inverseMass;
            transformB.position += correction * rbB.inverseMass;
        }
    }
}
