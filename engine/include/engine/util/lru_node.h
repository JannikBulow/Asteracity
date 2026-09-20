// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_UTIL_LRU_NODE_H
#define ASTERACITY_ENGINE_UTIL_LRU_NODE_H

namespace util {
    // designed for custom lru cache implementations. see ResourceManager and its associated resource classes for an example
    template<class T>
    struct LRUNode {
        LRUNode* prev = nullptr;
        LRUNode* next = nullptr;

        T value;
    };
}

#endif //ASTERACITY_ENGINE_UTIL_LRU_NODE_H
