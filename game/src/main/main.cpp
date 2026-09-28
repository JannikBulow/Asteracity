// Copyright 2026 Jannik Laugmand Bülow

#include <engine/addons/input/systems.h>

#include <engine/asset/animation.h>

#include <engine/world/components/animator.h>
#include <engine/world/components/camera.h>
#include <engine/world/components/renderer.h>
#include <engine/world/components/transform.h>

#include <engine/engine.h>

#include <ranges>

struct Actions {
    struct Move {
        using value_type = math::Vec2;
    };

    using types = std::tuple<Move>;
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

    engine.renderer().setDefaultFont(engine.resourceManager().createFont({"fonts/default.ttf"}, 18));

    engine.frameController().timer().setLimit(165);

    auto inputSystemPtr = std::make_unique<engine::input::InputSystem<Actions>>(engine.inputProvider());
    auto* inputSystem = inputSystemPtr.get();
    engine.addSystem(std::move(inputSystemPtr));

    inputSystem->bind<Actions::Move>({[](backend::IInputProvider& inputProvider) -> math::Vec2 {
        math::Vec2 result;
        if (inputProvider.isKeyDown(backend::Key::W)) result.y += 1.0f;
        if (inputProvider.isKeyDown(backend::Key::S)) result.y -= 1.0f;
        if (inputProvider.isKeyDown(backend::Key::A)) result.x -= 1.0f;
        if (inputProvider.isKeyDown(backend::Key::D)) result.x += 1.0f;
        return result;
    }});

    engine::Sprite rat = engine.assetManager().loadSprite({"sprites/rat.sprite"});
    engine::Sprite black = engine.assetManager().generateSprite({1, 1}, 1, 1, engine::ImageFormat::RGB8, [](int x, int y) { return math::Color::Black; });
    engine::Sprite grass1 = engine.assetManager().loadSprite({"sprites/grass_1.sprite"});

    engine::AnimationClip testAnimation = engine.assetManager().loadAnimation({"animations/test.animation"});

    engine.tileRegistry().registerTile(0, {std::move(black)});
    engine.tileRegistry().registerTile(1, {std::move(grass1)});

    auto scene = std::make_unique<engine::Scene>(engine.tileRegistry(), math::Vec2I{11, 11});

    for (int x = -5; x <= 5; x++) {
        for (int y = -5; y <= 5; y++) {
            scene->tileWorld().get({x, y}).id = 1;
        }
    }

    engine.pushScene(std::move(scene));

    engine::World& world = engine.activeScene().world();

    engine::Entity playerEntity = world.createEntity();
    world.addComponent<engine::CameraComponent>(playerEntity, engine.activeScene().getActiveCamera());
    engine::Transform& transform = world.addComponent<engine::Transform>(playerEntity);
    world.addComponent<engine::SpriteAnimator>(playerEntity, engine::Animation(testAnimation));
    world.addComponent<engine::SpriteRenderer>(playerEntity, testAnimation.getFrames().front().sprite);

    engine.setCallback(engine::Engine::UPDATE_CALLBACK, [inputSystem, &transform](engine::Engine& engine, float dt) {
        transform.position += inputSystem->value<Actions::Move>() * (dt * 5.0f);
    });

     return engine.main(argc, argv);
}