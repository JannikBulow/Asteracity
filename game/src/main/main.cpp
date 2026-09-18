// Copyright 2026 Jannik Laugmand Bülow

#include <engine/backend/backend.h>

#include <engine/backends/glfw/input_provider.h>
#include <engine/backends/glfw/window.h>

#include <engine/backends/miniaudio/audio_device.h>

#include <engine/backends/opengl/renderer.h>
#include <engine/backends/opengl/graphics_device.h>

#include <engine/backends/stb/asset_provider.h>

#include <engine/input/input_handler.h>

#include <engine/world/components/renderer.h>
#include <engine/world/components/transform.h>

#include <engine/engine.h>

#include <ranges>

enum class Action {
    Up = 0,
    Down,
    Left,
    Right,

    Count
};

struct MovementComponent {};

class MovementSystem : public engine::ISystem {
public:
    explicit MovementSystem(engine::InputHandler<Action>& inputHandler)
        : mInputHandler(inputHandler) {}

    void update(engine::Engine& engine, float dt) override {
        mInputHandler.update();

        for (auto [entity, transform, movement] : engine.activeScene().world().viewEntities<engine::Transform, MovementComponent>()) {
            if (mInputHandler.isDown(Action::Up)) transform.position.y += 10.0f * dt;
            if (mInputHandler.isDown(Action::Down)) transform.position.y -= 10.0f * dt;
            if (mInputHandler.isDown(Action::Left)) transform.position.x -= 10.0f * dt;
            if (mInputHandler.isDown(Action::Right)) transform.position.x += 10.0f * dt;
        }
    }

private:
    engine::InputHandler<Action>& mInputHandler;
};

int main(int argc, char** argv) {
    engine::Engine engine({
        .window = {
            .width = 100,
            .height = 100,
            .title = "Asteracity"
        },
        .graphicsBackend = engine::GraphicsBackend::OpenGL,
        .audioBackend = engine::AudioBackend::Miniaudio
    });

    engine.frameController().timer().setLimit(165);

    engine::InputHandler<Action> input(engine.backend());
    input.setKeybind(Action::Up, engine::Key::W);
    input.setKeybind(Action::Down, engine::Key::S);
    input.setKeybind(Action::Left, engine::Key::A);
    input.setKeybind(Action::Right, engine::Key::D);

    engine.addSystem(std::make_unique<MovementSystem>(input));

    engine::Sprite rat = engine.assetManager().loadSprite({"sprites/rat.sprite"});
    engine::Sprite black = engine.assetManager().loadSprite({"sprites/black.sprite"});
    engine::Sprite grass1 = engine.assetManager().loadSprite({"sprites/grass_1.sprite"});

    engine.tileRegistry().registerTile(0, {std::move(black)});
    engine.tileRegistry().registerTile(1, {std::move(grass1)});

    auto scene = std::make_unique<engine::Scene>(engine.tileRegistry(), math::Vec2I{50, 50});
    scene->tileWorld().get({0, 0}).id = 1;

    engine.pushScene(std::move(scene));

    engine::World& world = engine.activeScene().world();

    engine::Entity playerEntity = world.createEntity();
    auto& playerTransform = world.addComponent<engine::Transform>(playerEntity);
    world.addComponent<engine::SpriteRenderer>(playerEntity, rat);
    world.addComponent<MovementComponent>(playerEntity);

    engine.setCallback(engine::Engine::UPDATE_CALLBACK, [&playerTransform](engine::Engine& engine, float dt) {
        engine.activeScene().activeCamera().position = playerTransform.position;
    });

     return engine.main(argc, argv);
}