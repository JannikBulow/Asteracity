// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_SCENE_SCENE_H
#define ASTERACITY_ENGINE_SCENE_SCENE_H

#include "engine/render/camera.h"

#include "engine/world/world.h"

namespace engine {
    class Scene {
    public:
        explicit Scene(Camera mainCamera = {});

        World& world() { return mWorld; }
        const World& world() const { return mWorld; }

        CameraHandle createCamera(Camera camera = {});
        void destroyCamera(CameraHandle camera);

        Camera& getCamera(CameraHandle camera);
        const Camera& getCamera(CameraHandle camera) const;

        CameraHandle getMainCamera() const;

        CameraHandle getActiveCamera() const;
        void setActiveCamera(CameraHandle camera);

        Camera& mainCamera() { return getCamera(getMainCamera()); }
        const Camera& mainCamera() const { return getCamera(getMainCamera()); }

        Camera& activeCamera() { return getCamera(getActiveCamera()); }
        const Camera& activeCamera() const { return getCamera(getActiveCamera()); }

    private:
        World mWorld;

        util::HandleStorage<Camera> mCameras;
        CameraHandle mMainCamera; // always exists
        CameraHandle mActiveCamera;
    };
}

#endif //ASTERACITY_ENGINE_SCENE_SCENE_H
