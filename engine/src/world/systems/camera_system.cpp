// Copyright 2026 Jannik Laugmand Bülow

#include "engine/world/components/camera.h"
#include "engine/world/components/transform.h"

#include "engine/world/systems/camera_system.h"

#include "engine/engine.h"

namespace engine {
    void CameraSystem::postUpdate(Engine& engine, float dt) {
        Scene& activeScene = engine.activeScene();
        for (auto [entity, transform, camera] : activeScene.world().viewEntities<Transform, CameraComponent>()) {
            activeScene.getCamera(camera.camera).position = transform.position;
        }
    }
}
