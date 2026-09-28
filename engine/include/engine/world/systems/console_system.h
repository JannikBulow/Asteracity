// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_WORLD_SYSTEMS_CONSOLE_SYSTEM_H
#define ASTERACITY_ENGINE_WORLD_SYSTEMS_CONSOLE_SYSTEM_H

#include "engine/backend/input_provider.h"

#include "engine/developer/console.h"

#include "engine/world/system.h"

namespace engine {
    class ConsoleSystem : public ISystem {
    public:
        void update(Engine& engine, float dt) override;
        void renderUI(Engine& engine) override;

    private:
        struct KeyState {
            bool current;
            bool previous;

            bool isPressed() const;

            void update(Engine& engine, backend::Key key);
        };

        std::string mPendingInput;
        size_t mCursorPosition = 0;

        KeyState mEnter{};
        KeyState mBackspace{};
        KeyState mF1{};

        void handleInput(Engine& engine);
    };
}

#endif //ASTERACITY_ENGINE_WORLD_SYSTEMS_CONSOLE_SYSTEM_H
