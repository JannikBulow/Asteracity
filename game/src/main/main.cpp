// Copyright 2026 Jannik Laugmand Bülow

#include "asteracity/character/character_controller.h"
#include "asteracity/character/character_intent.h"
#include "asteracity/character/character_stats.h"

#include "asteracity/player/player_input.h"

#include <engine/addons/input/system.h>

#include <engine/asset/animation.h>

#include <engine/world/components/animator.h>
#include <engine/world/components/camera.h>
#include <engine/world/components/renderer.h>
#include <engine/world/components/transform.h>
#include <engine/world/components/velocity.h>

#include <engine/engine.h>

#include <ranges>

int main(int argc, char** argv) {
    using namespace asteracity;

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

    engine.addSystem(std::make_unique<PlayerInputSystem>(engine, PlayerBindings<engine::input::Key>::DefaultBindings()));

    engine.addSystem(std::make_unique<CharacterControllerSystem>());

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
    world.addComponent<engine::Transform>(playerEntity);
    world.addComponent<engine::Velocity>(playerEntity);
    world.addComponent<engine::SpriteAnimator>(playerEntity, engine::Animation(testAnimation));
    world.addComponent<engine::SpriteRenderer>(playerEntity, testAnimation.getFrames().front().sprite);
    world.addComponent<CharacterStats>(playerEntity, 5.0f);
    world.addComponent<CharacterIntent>(playerEntity);
    world.addComponent<PlayerControlled>(playerEntity);

     return engine.main(argc, argv);
}