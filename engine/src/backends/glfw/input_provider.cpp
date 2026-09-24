// Copyright 2026 Jannik Laugmand Bülow

#include "engine/backends/glfw/input_provider.h"

#include "GLFW/glfw3.h"

namespace backend {
    static constexpr int glfwKeyMap[static_cast<size_t>(Key::_keyCount)] = {
        GLFW_KEY_UNKNOWN,

        GLFW_KEY_1,
        GLFW_KEY_2,
        GLFW_KEY_3,
        GLFW_KEY_4,
        GLFW_KEY_5,
        GLFW_KEY_6,
        GLFW_KEY_7,
        GLFW_KEY_8,
        GLFW_KEY_9,
        GLFW_KEY_0,

        GLFW_KEY_A,
        GLFW_KEY_B,
        GLFW_KEY_C,
        GLFW_KEY_D,
        GLFW_KEY_E,
        GLFW_KEY_F,
        GLFW_KEY_G,
        GLFW_KEY_H,
        GLFW_KEY_I,
        GLFW_KEY_J,
        GLFW_KEY_K,
        GLFW_KEY_L,
        GLFW_KEY_M,
        GLFW_KEY_N,
        GLFW_KEY_O,
        GLFW_KEY_P,
        GLFW_KEY_Q,
        GLFW_KEY_R,
        GLFW_KEY_S,
        GLFW_KEY_T,
        GLFW_KEY_U,
        GLFW_KEY_V,
        GLFW_KEY_W,
        GLFW_KEY_X,
        GLFW_KEY_Y,
        GLFW_KEY_Z,

        GLFW_KEY_SPACE,
        GLFW_KEY_ESCAPE,
        GLFW_KEY_ENTER,
        GLFW_KEY_TAB,
        GLFW_KEY_BACKSPACE,

        GLFW_KEY_F1,
        GLFW_KEY_F2,
        GLFW_KEY_F3,
        GLFW_KEY_F4,
        GLFW_KEY_F5,
        GLFW_KEY_F6,
        GLFW_KEY_F7,
        GLFW_KEY_F8,
        GLFW_KEY_F9,
        GLFW_KEY_F10,
        GLFW_KEY_F11,
        GLFW_KEY_F12,
        GLFW_KEY_F13,
        GLFW_KEY_F14,
        GLFW_KEY_F15,
        GLFW_KEY_F16,
        GLFW_KEY_F17,
        GLFW_KEY_F18,
        GLFW_KEY_F19,
        GLFW_KEY_F20,
        GLFW_KEY_F21,
        GLFW_KEY_F22,
        GLFW_KEY_F23,
        GLFW_KEY_F24,
        GLFW_KEY_F25,
    };


    GLFWInputProvider::GLFWInputProvider(GLFWWindow& window)
        : mWindow(window.mWindow) {
        glfwSetWindowUserPointer(mWindow, this);

        glfwSetCharCallback(mWindow, [](GLFWwindow* window, unsigned int glfwCodepoint) {
            unicode::codepoint codepoint = static_cast<unicode::codepoint>(glfwCodepoint);
            GLFWInputProvider* input = static_cast<GLFWInputProvider*>(glfwGetWindowUserPointer(window));

            input->mInputQueue.push(codepoint);
        });

        glfwSetScrollCallback(mWindow, [](GLFWwindow* wwindow, double x, double y) {
            GLFWInputProvider* input = static_cast<GLFWInputProvider*>(glfwGetWindowUserPointer(wwindow));

            input->mMouseScroll.x += x;
            input->mMouseScroll.y += y;
        });
    }

    void GLFWInputProvider::pollEvents() {
        while (!mInputQueue.empty()) mInputQueue.pop(); // TODO: clear?
        mMouseScroll = math::Vec2D::Zero();
    }

    std::optional<char> GLFWInputProvider::getCharPressed() {
        std::optional<unicode::codepoint> codepoint = getUnicodePressed();
        if (!codepoint) return std::nullopt;
        return unicode::ToAscii(*codepoint);
    }

    std::optional<unicode::codepoint> GLFWInputProvider::getUnicodePressed() {
        if (mInputQueue.empty()) return std::nullopt;
        unicode::codepoint codepoint = mInputQueue.front();
        mInputQueue.pop();
        return codepoint;
    }

    bool GLFWInputProvider::isKeyDown(Key key) {
        return glfwGetKey(mWindow, glfwKeyMap[static_cast<size_t>(key)]) == GLFW_PRESS;
    }

    bool GLFWInputProvider::isKeyUp(Key key) {
        return glfwGetKey(mWindow, glfwKeyMap[static_cast<size_t>(key)]) == GLFW_RELEASE;
    }

    math::Vec2 GLFWInputProvider::getMousePosition() {
        double x, y;
        glfwGetCursorPos(mWindow, &x, &y);
        return {static_cast<float>(x), static_cast<float>(y)};
    }

    bool GLFWInputProvider::isMouseButtonDown(int button) {
        return glfwGetMouseButton(mWindow, button) == GLFW_PRESS; // glfw and this engine use the same mouse button numbers
    }

    bool GLFWInputProvider::isMouseButtonUp(int button) {
        return glfwGetMouseButton(mWindow, button) == GLFW_RELEASE; // glfw and this engine use the same mouse button numbers
    }

    math::Vec2 GLFWInputProvider::getMouseScroll() {
        return math::Vec2(mMouseScroll);
    }
}
