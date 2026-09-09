// Copyright 2026 Jannik Laugmand Bülow

#include "engine/scene/scene.h"

namespace engine {
    Scene::Scene(TileRegistry& tileRegistry, math::Vec2I worldSize, Camera mainCamera)
        : mTileWorld(tileRegistry, worldSize.x, worldSize.y) {
        mMainCamera = createCamera(mainCamera);
        mActiveCamera = mMainCamera;
    }

    CameraHandle Scene::createCamera(Camera camera) {
        return mCameras.create(camera);
    }

    void Scene::destroyCamera(CameraHandle camera) {
        if (camera == mMainCamera) throw util::GameException();
        if (camera == mActiveCamera) throw util::GameException();

        mCameras.destroy(camera);
    }

    Camera& Scene::getCamera(CameraHandle camera) {
        return mCameras.get(camera);
    }

    const Camera& Scene::getCamera(CameraHandle camera) const {
        return mCameras.get(camera);
    }

    CameraHandle Scene::getMainCamera() const {
        return mMainCamera;
    }

    CameraHandle Scene::getActiveCamera() const {
        return mActiveCamera;
    }

    void Scene::setActiveCamera(CameraHandle camera) {
        mActiveCamera = camera;
    }
}
