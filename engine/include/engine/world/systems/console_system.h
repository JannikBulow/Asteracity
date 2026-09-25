// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_SYSTEMS_CONSOLE_SYSTEM_H
#define ASTERACITY_ENGINE_WORLD_SYSTEMS_CONSOLE_SYSTEM_H

#include "engine/developer/console.h"

#include "engine/input/input_handler.h"

#include "engine/world/system.h"

namespace engine {
    class ConsoleSystem : public ISystem {
    public:
        explicit ConsoleSystem(Engine& engine);

        void update(Engine& engine, float dt) override;
        void renderUI(Engine& engine) override;

    private:
        enum class Action {
            Backspace,
            Enter,
            ToggleConsole,

            Count
        };

        InputHandler<Action, true> mInputHandler;

        std::string mPendingInput;
        size_t mCursorPosition = 0;

        void handleInput(Engine& engine);
    };
}

#endif //ASTERACITY_ENGINE_WORLD_SYSTEMS_CONSOLE_SYSTEM_H
