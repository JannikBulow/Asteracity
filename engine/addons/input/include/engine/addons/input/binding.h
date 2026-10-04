// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_ADDONS_INPUT_BINDING_H
#define ASTERACITY_ENGINE_ADDONS_INPUT_BINDING_H

#include <engine/backend/input_provider.h>

#include <functional>

namespace engine::input {
    template<class T>
    struct Binding {
        using value_type = T;

        std::function<T(backend::IInputProvider&)> evaluate;
    };

    using Key = backend::Key;
}

#endif //ASTERACITY_ENGINE_ADDONS_INPUT_BINDING_H
