// Copyright 2026 Jannik Laugmand Bülow

#include "asteracity/character/character_intent.h"

#include "asteracity/player/player_input.h"

namespace asteracity {
    template<>
    PlayerBindings<engine::input::Key> PlayerBindings<engine::input::Key>::DefaultBindings() {
        using engine::input::Key;
        return {
            .moveUp = Key::W,
            .moveDown = Key::S,
            .moveLeft = Key::A,
            .moveRight = Key::D,
        };
    }

    PlayerInputSystem::PlayerInputSystem(engine::Engine& engine, PlayerBindings<engine::input::Key> keyboardBindings)
        : InputSystem(engine.inputProvider())
        , mKeyboardBindings(keyboardBindings) {
        bind<PlayerActions::Move>({[this](backend::IInputProvider& inputProvider) -> math::Vec2 {
            math::Vec2 result = math::Vec2::Zero();
            if (inputProvider.isKeyDown(mKeyboardBindings.moveUp)) result.y += 1.0f;
            if (inputProvider.isKeyDown(mKeyboardBindings.moveDown)) result.y -= 1.0f;
            if (inputProvider.isKeyDown(mKeyboardBindings.moveLeft)) result.x -= 1.0f;
            if (inputProvider.isKeyDown(mKeyboardBindings.moveRight)) result.x += 1.0f;
            return result;
        }});
    }

    void PlayerInputSystem::update(engine::Engine& engine, float dt) {
        InputSystem::update(engine, dt);

        for (auto [entity, controlled, intent] : engine.activeScene().world().viewEntities<PlayerControlled, CharacterIntent>()) {
            intent.movement = value<PlayerActions::Move>();
        }
    }
}
