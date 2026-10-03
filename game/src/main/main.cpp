// Copyright 2026 Jannik Laugmand Bülow

#include <engine/addons/input/system.h>

#include <engine/asset/animation.h>

#include <engine/world/components/animator.h>
#include <engine/world/components/camera.h>
#include <engine/world/components/renderer.h>
#include <engine/world/components/transform.h>
#include <engine/world/components/velocity.h>

#include <engine/engine.h>

#include <ranges>

struct Actions {
    struct Move {
        using value_type = math::Vec2;
    };

    using types = std::tuple<Move>;
};

// simple tag to tell the system that the entity has movement
struct CharacterController {
    float speed;
};

class CharacterControllerSystem : public engine::ISystem {
public:
    // takes a normalized movement system, usually just provided by the input addon via something like `inputSystem->value<Actions::Move>()`
    explicit CharacterControllerSystem(const math::Vec2& movementVector)
        : mMovementVector(movementVector) {}

    void update(engine::Engine& engine, float dt) override {
        for (auto [entity, movement, velocity] : engine.activeScene().world().viewEntities<CharacterController, engine::Velocity>()) {
            velocity.linear = mMovementVector * movement.speed;
        }
    }

private:
    const math::Vec2& mMovementVector;
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

    engine.addSystem(std::make_unique<CharacterControllerSystem>(inputSystem->value<Actions::Move>()));

    inputSystem->bind<Actions::Move>({[](backend::IInputProvider& inputProvider) -> math::Vec2 {
        math::Vec2 result;
        if (inputProvider.isKeyDown(backend::Key::W)) result.y += 1.0f;
        if (inputProvider.isKeyDown(backend::Key::S)) result.y -= 1.0f;
        if (inputProvider.isKeyDown(backend::Key::A)) result.x -= 1.0f;
        if (inputProvider.isKeyDown(backend::Key::D)) result.x += 1.0f;
        return result;
    }});

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
    world.addComponent<CharacterController>(playerEntity, 5.0f);

     return engine.main(argc, argv);
}