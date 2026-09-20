// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_RESOURCE_RECLAIM_NODE_H
#define ASTERACITY_ENGINE_RESOURCE_RECLAIM_NODE_H

#include "engine/util/lru_node.h"

#include <cstddef>

namespace engine {
    class ResourceManager;

    enum class ReclaimKind : uint8_t {
        FontCPU,
        FontGPU,
        SoundCPU,
        TextureCPU,
        TextureGPU,
    };

    struct ReclaimData {
        size_t size;
        void* resource;
        ReclaimKind kind;
    };

    using ReclaimNode = util::LRUNode<ReclaimData>;
}

#endif //ASTERACITY_ENGINE_RESOURCE_RECLAIM_NODE_H
