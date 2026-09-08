// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_BACKEND_BACKEND_H
#define ASTERACITY_ENGINE_BACKEND_BACKEND_H

#include "engine/backend/asset_provider.h"
#include "engine/backend/audio_device.h"
#include "engine/backend/graphics_device.h"
#include "engine/backend/input_provider.h"
#include "engine/backend/renderer.h"
#include "engine/backend/window.h"

#include <memory>

namespace backend {
    struct Backend {
        IAssetProvider& assetProvider;
        IAudioDevice& audio;
        IGraphicsDevice& gpu;
        IInputProvider& inputProvider;
        IRenderer& renderer;
        IWindow& window;
    };

    struct OwningBackend {
        std::unique_ptr<IAssetProvider> assetProvider;
        std::unique_ptr<IAudioDevice> audio;
        std::unique_ptr<IGraphicsDevice> gpu;
        std::unique_ptr<IInputProvider> inputProvider;
        std::unique_ptr<IRenderer> renderer;
        std::unique_ptr<IWindow> window;

        operator Backend() const {
            return {
                .assetProvider = *assetProvider,
                .audio = *audio,
                .gpu = *gpu,
                .inputProvider = *inputProvider,
                .renderer = *renderer,
                .window = *window,
            };
        }
    };
}

#endif //ASTERACITY_ENGINE_BACKEND_BACKEND_H
