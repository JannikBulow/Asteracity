// Copyright 2026 Jannik Laugmand Bülow

// Collection of definitions defined by macros

#ifndef ASTERACITY_ENGINE_COMPILE_OPTIONS_H
#define ASTERACITY_ENGINE_COMPILE_OPTIONS_H

#ifdef ASTERACITY_TARGET_MULTITHREADED
#include <atomic>
#endif

namespace options {
#ifdef ASTERACITY_TARGET_MULTITHREADED
    template<class T>
    using Atomic = std::atomic<T>;
#else
    template<class T>
    using Atomic = T;
#endif
}

#endif //ASTERACITY_ENGINE_COMPILE_OPTIONS_H
