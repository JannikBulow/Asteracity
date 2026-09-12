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

    engine::Sprite rat(engine.resourceManager().createTexture({"images/rat.png"}), math::Vec2::One());
    engine::Sprite black(engine.resourceManager().createTexture({"images/black.png"}), math::Vec2::One());
    engine::Sprite grass1(engine.resourceManager().createTexture({"images/grass_1.png"}), math::Vec2::One());

    engine.tileRegistry().registerTile(0, {std::move(black)});
    engine.tileRegistry().registerTile(1, {std::move(grass1)});

    auto scene = std::make_unique<engine::Scene>(engine.tileRegistry(), math::Vec2I{50, 50});
    scene->tileWorld().get({0, 0}).id = 1;

    engine.pushScene(std::move(scene));

    engine::World& world = engine.activeScene().world();

    engine::Entity playerEntity = world.createEntity();
    world.addComponent<engine::SpriteRenderer>(playerEntity, rat);
    world.addComponent<engine::Transform>(playerEntity);

    auto& playerTransform = *world.getComponent<engine::Transform>(playerEntity);

    engine.setCallback(engine::Engine::UPDATE_CALLBACK, [&input, &playerTransform](engine::Engine& engine, float dt) {
        input.update();

        if (input.isDown(Action::Up)) playerTransform.position.y += 10.0f * dt;
        if (input.isDown(Action::Down)) playerTransform.position.y -= 10.0f * dt;
        if (input.isDown(Action::Left)) playerTransform.position.x -= 10.0f * dt;
        if (input.isDown(Action::Right)) playerTransform.position.x += 10.0f * dt;

        engine.activeScene().activeCamera().position = playerTransform.position;
    });

     return engine.main(argc, argv);
}