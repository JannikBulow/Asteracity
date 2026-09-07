// Copyright 2026 Jannik Laugmand Bülow

#include "engine/engine.h"

namespace engine {
    Engine::Engine(const Config& config)
        : mOwningBackend(config.realizeBackend())
        , mBackend(mOwningBackend)
        , mFrameController(mBackend)
        , mResourceManager(mBackend)
        , mRenderer(mBackend)
        , mAudioDevice(mBackend)
        , mBackgroundColor(math::Color::White) {}

    Engine::Engine(const backend::Backend& backend)
        : mBackend(backend)
        , mFrameController(mBackend)
        , mResourceManager(mBackend)
        , mRenderer(mBackend)
        , mAudioDevice(mBackend)
        , mBackgroundColor(math::Color::White) {}

    void Engine::setBackgroundColor(math::Color color) {
        mBackgroundColor = color;
    }

    void Engine::setCallback(CallbackID id, std::function<void(Engine&, float)> callback) {
        mCallbacks[id] = std::move(callback);
    }

    int Engine::main(std::span<std::string_view> args) {
        mFrameController.setCamera(mCamera);
        mFrameController.timer().start();
        while (!window().shouldClose()) {
            mFrameController.execute(
                [this](float dt) {
                    call(UPDATE_CALLBACK, dt);
                    mAudioDevice.update(dt);
                },
                [this](float dt) {
                    mRenderer.clear(mBackgroundColor);
                },
                [this](float dt) {
                    call(WORLD_RENDER_CALLBACK, dt);
                },
                [this](float dt) {
                    call(UI_RENDER_CALLBACK, dt);
                }
            );
        }

        return 0;
    }

    void Engine::call(CallbackID id, float dt) {
        if (mCallbacks[id]) mCallbacks[id](*this, dt);
    }
}
