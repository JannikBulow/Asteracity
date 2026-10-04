// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_PLAYER_PLAYER_INPUT_H
#define ASTERACITY_PLAYER_PLAYER_INPUT_H

#include <engine/addons/input/system.h>

#include <engine/util/math.h>

#include <engine/engine.h>

#include <tuple>

namespace asteracity {
    struct PlayerActions {
        struct Move {
            using value_type = math::Vec2;
        };

        using types = std::tuple<Move>;
    };

    template<class BindingPrimitive>
    struct PlayerBindings {
        BindingPrimitive moveUp;
        BindingPrimitive moveDown;
        BindingPrimitive moveLeft;
        BindingPrimitive moveRight;

        // this function is defined in a source file with only the relevant types
        static PlayerBindings DefaultBindings();
    };

    struct PlayerControlled {};

    class PlayerInputSystem : public engine::input::InputSystem<PlayerActions> {
    public:
        PlayerInputSystem(engine::Engine& engine, PlayerBindings<engine::input::Key> keyboardBindings);

        void update(engine::Engine& engine, float dt) override;

    private:
        PlayerBindings<engine::input::Key> mKeyboardBindings;
    };
}

#endif //ASTERACITY_PLAYER_PLAYER_INPUT_H
