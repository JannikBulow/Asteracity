// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_RENDER_CAMERA_H
#define ASTERACITY_ENGINE_RENDER_CAMERA_H

#include "engine/backend/renderer.h"

#include "engine/util/handle.h"

namespace engine {
    using Camera = backend::Camera2D;
    using CameraHandle = util::Handle<Camera>; // for scene
}

#endif //ASTERACITY_ENGINE_RENDER_CAMERA_H
