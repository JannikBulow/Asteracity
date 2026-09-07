// Copyright 2026 Jannik Laugmand Bülow

#ifndef UNNAMEDGAME_ENGINE_CONFIG_H
#define UNNAMEDGAME_ENGINE_CONFIG_H

#include "engine/backend/backend.h"

#include <string>

namespace engine {
    enum class GraphicsBackend : unsigned char {
        OpenGL,
    };

    enum class AudioBackend : unsigned char {
        Miniaudio
    };

    struct Config {
        struct Window {
            int width;
            int height;
            std::string title;
        };

        Window window;
        GraphicsBackend graphicsBackend;
        AudioBackend audioBackend;

        backend::OwningBackend realizeBackend() const;
    };
}

#endif //UNNAMEDGAME_ENGINE_CONFIG_H
