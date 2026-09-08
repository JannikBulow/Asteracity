// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_BACKENDS_GLFW_WINDOW_H
#define ASTERACITY_ENGINE_BACKENDS_GLFW_WINDOW_H

#include "engine/backend/window.h"

struct GLFWwindow;

namespace backend {
    class GLFWWindow : public IWindow {
        friend class GLFWInputProvider;
    public:
        GLFWWindow(int width, int height, const char* title);
        ~GLFWWindow() override;

        bool shouldClose() const override;
        void pollEvents() override;
        void swapBuffers() override;

        math::Vec2I getFramebufferSize() const override;
        math::Vec2I getWindowSize() const override;

    private:
        GLFWwindow* mWindow;
    };
}

#endif //ASTERACITY_ENGINE_BACKENDS_GLFW_WINDOW_H
