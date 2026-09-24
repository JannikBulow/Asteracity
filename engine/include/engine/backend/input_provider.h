// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_BACKEND_INPUT_PROVIDER_H
#define ASTERACITY_ENGINE_BACKEND_INPUT_PROVIDER_H

#include "engine/util/math.h"
#include "engine/util/unicode.h"

#include <optional>

namespace backend {
    enum class Key {
        Unknown = 0,

        One,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Zero,

        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        Space,
        Escape,
        Enter,
        Tab,
        Backspace,

        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,
        F25,

        _keyCount
    };

    class IInputProvider {
    public:
        virtual ~IInputProvider() = default;

        virtual void pollEvents() = 0;

        virtual std::optional<char> getCharPressed() = 0;
        virtual std::optional<unicode::codepoint> getUnicodePressed() = 0;

        virtual bool isKeyDown(Key key) = 0;
        virtual bool isKeyUp(Key key) = 0;

        virtual math::Vec2 getMousePosition() = 0;
        virtual bool isMouseButtonDown(int button) = 0;
        virtual bool isMouseButtonUp(int button) = 0;

        virtual math::Vec2 getMouseScroll() = 0;
    };
}

#endif //ASTERACITY_ENGINE_BACKEND_INPUT_PROVIDER_H
