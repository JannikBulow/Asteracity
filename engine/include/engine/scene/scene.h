// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_SCENE_SCENE_H
#define ASTERACITY_ENGINE_SCENE_SCENE_H

#include "engine/render/camera.h"

#include "engine/world/tiles/tile_world.h"

#include "engine/world/world.h"

namespace engine {
    class Scene {
    public:
        Scene(TileRegistry& tileRegistry, math::Vec2I worldSize, Camera mainCamera = {});

        World& world() { return mWorld; }
        TileWorld& tileWorld() { return mTileWorld; }

        const World& world() const { return mWorld; }
        const TileWorld& tileWorld() const { return mTileWorld; }

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
        TileWorld mTileWorld;

        util::HandleStorage<Camera> mCameras;
        CameraHandle mMainCamera; // always exists
        CameraHandle mActiveCamera;
    };
}

#endif //ASTERACITY_ENGINE_SCENE_SCENE_H
