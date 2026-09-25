// Copyright 2026 Jannik Laugmand Bülow

#include <engine/asset/animation.h>

#include <engine/input/input_handler.h>

#include <engine/world/components/animator.h>
#include <engine/world/components/camera.h>
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

    engine::InputHandler<Action> input(engine.backend(), engine.console());
    input.setKeybind(Action::Up, engine::Key::W);
    input.setKeybind(Action::Down, engine::Key::S);
    input.setKeybind(Action::Left, engine::Key::A);
    input.setKeybind(Action::Right, engine::Key::D);

    engine.addSystem(std::make_unique<MovementSystem>(input));

    engine::Sprite rat = engine.assetManager().loadSprite({"sprites/rat.sprite"});
    engine::Sprite black = engine.assetManager().loadSprite({"sprites/black.sprite"});
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
    world.addComponent<engine::SpriteAnimator>(playerEntity, engine::Animation(testAnimation));
    world.addComponent<engine::SpriteRenderer>(playerEntity, testAnimation.getFrames().front().sprite);
    world.addComponent<MovementComponent>(playerEntity);

     return engine.main(argc, argv);
}