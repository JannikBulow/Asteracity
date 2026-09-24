// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_BACKENDS_GLFW_INPUT_PROVIDER_H
#define ASTERACITY_ENGINE_BACKENDS_GLFW_INPUT_PROVIDER_H

#include "engine/backend/input_provider.h"

#include "engine/backends/glfw/window.h"

#include <queue>

namespace backend {
    class GLFWInputProvider : public IInputProvider {
    public:
        explicit GLFWInputProvider(GLFWWindow& window);

        void pollEvents() override;

        std::optional<char> getCharPressed() override;
        std::optional<unicode::codepoint> getUnicodePressed() override;

        bool isKeyDown(Key key) override;
        bool isKeyUp(Key key) override;

        math::Vec2 getMousePosition() override;
        bool isMouseButtonDown(int button) override;
        bool isMouseButtonUp(int button) override;

        math::Vec2 getMouseScroll() override;

    private:
        GLFWwindow* mWindow; // unowned!!!

        std::queue<unicode::codepoint> mInputQueue;

        math::Vec2D mMouseScroll = math::Vec2D::Zero();
    };
}

#endif //ASTERACITY_ENGINE_BACKENDS_GLFW_INPUT_PROVIDER_H
