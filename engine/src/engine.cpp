// Copyright 2026 Jannik Laugmand Bülow

#include "engine/engine.h"

namespace engine {
    Engine::Engine(const Config& config)
        : mOwningBackend(config.realizeBackend())
        , mBackend(mOwningBackend)
        , mFrameController(mBackend)
        , mResourceManager(mBackend)
        , mRenderer(mBackend)
        , mAudioDevice(mBackend) {}

    Engine::Engine(const backend::Backend& backend)
        : mBackend(backend)
        , mFrameController(mBackend)
        , mResourceManager(mBackend)
        , mRenderer(mBackend)
        , mAudioDevice(mBackend) {}

    void Engine::pushScene(std::unique_ptr<Scene> scene) {
        mSceneStack.push_back(std::move(scene));
    }

    void Engine::popScene() {
        mSceneStack.pop_back();
    }

    void Engine::clearSceneStack() {
        mSceneStack.clear();
    }

    void Engine::addSystem(std::unique_ptr<ISystem> system) {
        mSystems.push_back(std::move(system));
    }

    void Engine::setCallback(CallbackID id, std::function<void(Engine&, float)> callback) {
        mCallbacks[id] = std::move(callback);
    }

    int Engine::main(std::span<std::string_view> args) {
        std::string_view noSceneMessage = "Error: scene stack empty"; //TODO: default font, then render this text below in the mSceneStack.empty condition

        mFrameController.timer().start();
        while (!window().shouldClose()) {
            if (mSceneStack.empty()) {
                mFrameController.execute(
                    {},
                    [](float dt) {},
                    [this](float dt) {
                        mRenderer.clear(math::Color::Blue);
                    },
                    [](float dt) {},
                    [this](float dt) {

                    }
                );
                continue;
            }

            Scene& activeScene = this->activeScene();
            Camera& activeCamera = activeScene.activeCamera();

            mFrameController.execute(
                activeCamera,
                [this](float dt) {
                    preUpdate(dt);
                    update(dt);
                    postUpdate(dt);
                },
                [this, &activeCamera](float dt) {
                    mRenderer.clear(activeCamera.backgroundColor);
                },
                [this](float dt) {
                    render();
                },
                [this](float dt) {
                    renderUI();
                }
            );
        }

        return 0;
    }

    void Engine::call(CallbackID id, float dt) {
        if (mCallbacks[id]) mCallbacks[id](*this, dt);
    }

    void Engine::preUpdate(float dt) {
        std::apply([&](auto&... systems) {
            (systems.preUpdate(*this, dt), ...);
        }, mBuiltinSystems);

        for (auto& system : mSystems) {
            system->preUpdate(*this, dt);
        }
    }

    void Engine::update(float dt) {
        std::apply([&](auto&... systems) {
            (systems.update(*this, dt), ...);
        }, mBuiltinSystems);

        for (auto& system : mSystems) {
            system->update(*this, dt);
        }

        call(UPDATE_CALLBACK, dt);
    }

    void Engine::postUpdate(float dt) {
        std::apply([&](auto&... systems) {
            (systems.postUpdate(*this, dt), ...);
        }, mBuiltinSystems);

        for (auto& system : mSystems) {
            system->preUpdate(*this, dt);
        }

        mAudioDevice.update(dt);
    }

    void Engine::render() {
        std::apply([&](auto&... systems) {
            (systems.render(*this), ...);
        }, mBuiltinSystems);

        for (auto& system : mSystems) {
            system->render(*this);
        }

        call(WORLD_RENDER_CALLBACK, 0.0f);
    }

    void Engine::renderUI() {
        std::apply([&](auto&... systems) {
            (systems.renderUI(*this), ...);
        }, mBuiltinSystems);

        for (auto& system : mSystems) {
            system->renderUI(*this);
        }

        call(UI_RENDER_CALLBACK, 0.0f);
    }
}
