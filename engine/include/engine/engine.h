// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_ENGINE_H
#define ASTERACITY_ENGINE_ENGINE_H

#include "engine/backend/backend.h"

#include "engine/render/frame_controller.h"
#include "engine/render/renderer.h"

#include "engine/resource/resource_manager.h"

#include "engine/scene/scene.h"

#include "engine/sound/audio_device.h"

#include "engine/config.h"

#include <array>
#include <functional>
#include <memory>
#include <span>

namespace engine {
    class Engine {
    public:
        enum CallbackID {
            UPDATE_CALLBACK = 0,
            WORLD_RENDER_CALLBACK,
            UI_RENDER_CALLBACK,

            _count
        };

        explicit Engine(const Config& config);

        // for advanced users and people who make their own backends
        explicit Engine(const backend::Backend& backend);

        backend::Backend& backend() { return mBackend; }
        backend::IAssetProvider& assetProvider() const { return mBackend.assetProvider; }
        backend::IAudioDevice& audioBackend() const { return mBackend.audio; }
        backend::IGraphicsDevice& gpu() const { return mBackend.gpu; }
        backend::IInputProvider& inputProvider() const { return mBackend.inputProvider; }
        backend::IRenderer& rendererBackend() const { return mBackend.renderer; }
        backend::IWindow& window() const { return mBackend.window; }

        FrameController& frameController() { return mFrameController; }
        Renderer& renderer() { return mRenderer; }
        ResourceManager& resourceManager() { return mResourceManager; }
        AudioDevice& audioDevice() { return mAudioDevice; }

        const FrameController& frameController() const { return mFrameController; }
        const Renderer& renderer() const { return mRenderer; }
        const ResourceManager& resourceManager() const { return mResourceManager; }
        const AudioDevice& audioDevice() const { return mAudioDevice; }

        Scene& activeScene() { return *mSceneStack.back(); }
        const Scene& activeScene() const { return *mSceneStack.back(); }

        void pushScene(std::unique_ptr<Scene> scene);
        void popScene();
        void clearSceneStack();

        void setCallback(CallbackID id, std::function<void(Engine&)> callback) { setCallback(id, [callback = std::move(callback)](Engine& engine, float) { callback(engine); }); }
        void setCallback(CallbackID id, std::function<void(Engine&, float)> callback);

        int main(int argc, char** argv) {
            std::vector<std::string_view> args;
            args.reserve(argc - 1);
            for (int i = 1; i < argc; i++) args.emplace_back(argv[i]);
            return main(args);
        }

        int main(std::span<std::string_view> args); // engine entry point. does any required internal setup and starts the main loop using configured callbacks

    private:
        backend::OwningBackend mOwningBackend{};
        backend::Backend mBackend;

        FrameController mFrameController;
        ResourceManager mResourceManager;
        Renderer mRenderer;
        AudioDevice mAudioDevice;

        std::vector<std::unique_ptr<Scene>> mSceneStack;

        std::array<std::function<void(Engine&, float)>, _count> mCallbacks{};

        void call(CallbackID id, float dt);
    };
}

#endif //ASTERACITY_ENGINE_ENGINE_H
