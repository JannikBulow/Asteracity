// Copyright 2026 Jannik Laugmand Bülow

#include "engine/world/systems/console_system.h"

#include "engine/engine.h"

namespace engine {
    ConsoleSystem::ConsoleSystem(Engine& engine)
        : mInputHandler(engine.backend(), engine.console()) {
        mInputHandler.setKeybind(Action::Backspace, Key::Backspace);
        mInputHandler.setKeybind(Action::Enter, Key::Enter);
        mInputHandler.setKeybind(Action::ToggleConsole, Key::F1);
    }

    void ConsoleSystem::update(Engine& engine, float dt) {
        mInputHandler.update();

        if (mInputHandler.isPressed(Action::ToggleConsole)) {
            engine.console().setVisible(!engine.console().isVisible());
        }

        if (!engine.console().isVisible()) return;

        handleInput(engine);

        Console& console = engine.console();
        if (console.mScrollOffset < 0) console.mScrollOffset = 0;
        if (console.mScrollOffset > console.mLog.size() - 1) console.mScrollOffset = console.mLog.size() - 1;
    }

    void ConsoleSystem::renderUI(Engine& engine) {
        if (!engine.console().isVisible()) return;

        math::Vec2I framebufferSize = engine.window().getFramebufferSize();
        math::Rect consoleBounds = math::Rect(math::RectI(0, framebufferSize.x, framebufferSize.y, framebufferSize.y - std::max(framebufferSize.y / 2, 80)));

        constexpr float fontSize = 18;
        constexpr float padding = 6;
        constexpr float lineSpacing = 2;
        constexpr float lineHeight = fontSize + lineSpacing;
        constexpr float inputHeight = 28;

        math::Rect inputBounds = consoleBounds;
        inputBounds.top = inputBounds.bottom + inputHeight;

        math::Rect logBounds = consoleBounds;
        logBounds.bottom = inputBounds.top;

        engine.renderer().drawRect(consoleBounds, math::Color::Black);
        engine.renderer().drawRectOutline(consoleBounds, 1.0f, math::Color::White);
        engine.renderer().drawRectOutline(inputBounds, 1.0f, math::Color::White);

        Console& console = engine.console();

        int visibleLines = std::max(0, static_cast<int>((logBounds.height() - padding * 2.0f) / lineHeight));
        if (visibleLines > 0) {
            int lineCount = static_cast<int>(console.mLog.size());

            if (lineCount > 0) {
                int newest = lineCount - 1 - console.getScrollOffset();
                newest = std::clamp(newest, 0, lineCount - 1);

                int oldest = std::max(0, newest - visibleLines + 1);

                float y = logBounds.bottom + padding;

                for (int i = newest; i >= oldest; i--) {
                    engine.renderer().drawText(console.mLog[i], {logBounds.left + padding, y}, fontSize, math::Color::White);
                    y += lineHeight;
                }
            }
        }

        float x = inputBounds.left + padding;
        float y = inputBounds.top - (inputBounds.height() - fontSize) * 0.5f - fontSize;

        engine.renderer().drawText(mPendingInput, {x, y}, fontSize, math::Color::White);

        std::string beforeCursor(mPendingInput.data(), mCursorPosition);

        math::Vec2 inputSizeBeforeCursor = engine.renderer().measureText(beforeCursor, fontSize);
        float cursorWidth = 1.0f;
        float cursorHeight = fontSize;

        float cursorX = x + inputSizeBeforeCursor.x;
        float cursorY = y;

        engine.renderer().drawRect({cursorX + cursorWidth * 0.5f, cursorY + cursorHeight * 0.5f}, {cursorWidth, cursorHeight}, math::Color::White);
    }

    void ConsoleSystem::handleInput(Engine& engine) {
        while (std::optional<char> c = engine.inputProvider().getCharPressed()) {
            mPendingInput.push_back(*c); //TODO: handle cursor
            mCursorPosition++;
        }

        //TODO: add that thing where you can hold backspace and it'll delete faster
        if (mInputHandler.isPressed(Action::Backspace) && !mPendingInput.empty()) {
            mPendingInput.pop_back(); //TODO: handle cursor
            mCursorPosition--;
        }

        if (mInputHandler.isPressed(Action::Enter)) {
            Console& console = engine.console();
            console.print(std::format("> {}", mPendingInput));
            console.execute(engine, mPendingInput);
            mPendingInput.clear();
        }

        engine.console().mScrollOffset += static_cast<int>(engine.inputProvider().getMouseScroll().y);
    }
}
