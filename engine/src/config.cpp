// Copyright 2026 Jannik Laugmand Bülow

#include "engine/backends/glfw/input_provider.h"
#include "engine/backends/glfw/window.h"

#include "engine/backends/miniaudio/audio_device.h"

#include "engine/backends/opengl/graphics_device.h"
#include "engine/backends/opengl/renderer.h"

#include "engine/backends/stb/asset_provider.h"

#include "engine/config.h"

namespace engine {
    backend::OwningBackend Config::realizeBackend() const {
        backend::OwningBackend backend;

        backend.assetProvider = std::make_unique<backend::StbAssetProvider>();

        switch (audioBackend) {
            case AudioBackend::Miniaudio:
                backend.audio = std::make_unique<backend::MiniaudioAudioDevice>();
                break;
        }

        switch (graphicsBackend) {
            case GraphicsBackend::OpenGL:
                backend.window = std::make_unique<backend::GLFWWindow>(window.width, window.height, window.title.c_str());
                backend.inputProvider = std::make_unique<backend::GLFWInputProvider>(*static_cast<backend::GLFWWindow*>(backend.window.get()));
                backend.gpu = std::make_unique<backend::OpenGLGraphicsDevice>();
                backend.renderer = std::make_unique<backend::OpenGLRenderer>(*backend.gpu, *backend.window);
                break;
        }

        return backend;
    }
}
