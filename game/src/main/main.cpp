// Copyright 2026 Jannik Laugmand Bülow

#include <engine/backend/backend.h>

#include <engine/backends/glfw/input_provider.h>
#include <engine/backends/glfw/window.h>

#include <engine/backends/miniaudio/audio_device.h>

#include <engine/backends/opengl/renderer.h>
#include <engine/backends/opengl/graphics_device.h>

#include <engine/backends/stb/asset_provider.h>

#include <engine/input/input_handler.h>

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

    engine::Font font = engine.resourceManager().createFont({"/usr/share/fonts/liberation", "LiberationSans-Regular.ttf"}, 24);

    engine::Sprite rat(engine.resourceManager().createTexture({"/home/jannik/Downloads", "rat.png"}));

    math::Vec2 playerPosition = math::Vec2::Zero();

    engine.setCallback(engine::Engine::UPDATE_CALLBACK, [&input, &playerPosition](engine::Engine& engine, float dt) {
        input.update();

        if (input.isDown(Action::Up)) playerPosition.y += 10.0f * dt;
        if (input.isDown(Action::Down)) playerPosition.y -= 10.0f * dt;
        if (input.isDown(Action::Left)) playerPosition.x -= 10.0f * dt;
        if (input.isDown(Action::Right)) playerPosition.x += 10.0f * dt;

        engine.camera().position = playerPosition;
    });

    engine.setCallback(engine::Engine::WORLD_RENDER_CALLBACK, [&font, &rat, &playerPosition](engine::Engine& engine) {
        engine::Renderer& renderer = engine.renderer();

        renderer.drawRect(math::Vec2::Zero(), math::Vec2::One(), math::Color::Blue);
        renderer.drawSprite(rat, playerPosition, {1.0f, 1.5f});
        renderer.drawText(font, "playa", {playerPosition.x, playerPosition.y + 1.0f}, 24, math::Color::Black, true);
    });

     return engine.main(argc, argv);
}