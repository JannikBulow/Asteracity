// Copyright 2026 Jannik Laugmand Bülow

#include "engine/resource/resource_manager.h"

#include <ranges>

namespace engine {
    ResourceManager::ResourceManager(backend::Backend& backend)
        : mBackend(backend) {}

    ResourceManager::~ResourceManager() {
        for (auto& sampler : mSamplers | std::views::values) {
            mBackend.gpu.destroySampler(sampler);
        }
    }

    Font ResourceManager::createFont(util::ResourceLocation location, int fontSize, const unicode::codepoint* codepoints, int codePointCount, std::optional<SamplerDescriptor> sampler) {
        FontKey key(std::move(location), std::move(sampler), fontSize, codepoints, codePointCount);
        auto it = mFonts.find(key);
        if (it != mFonts.end()) return Font(&it->second);

        auto [it2, success] = mFonts.emplace(std::move(key), this);
        if (!success) throw util::GameException();

        it2->second.location = &it2->first.location;
        if (it2->first.sampler) it2->second.samplerDesc = &*it2->first.sampler;
        it2->second.fontSize = fontSize;
        it2->second.codepoints = codepoints; // TODO: clone later. lazy now
        it2->second.codePointCount = codePointCount;

        return Font(&it2->second);
    }

    Sound ResourceManager::createSound(util::ResourceLocation location) {
        auto it = mSounds.find(location);
        if (it != mSounds.end()) return Sound(&it->second);

        auto [it2, success] = mSounds.emplace(std::move(location), this);
        if (!success) throw util::GameException();

        it2->second.location = &it2->first;

        return Sound(&it2->second);
    }

    Texture ResourceManager::createTexture(util::ResourceLocation location, std::optional<SamplerDescriptor> sampler) {
        TextureKey key(std::move(location), std::move(sampler));
        auto it = mTextures.find(key);
        if (it != mTextures.end()) return Texture(&it->second);

        auto [it2, success] = mTextures.emplace(std::move(key), this);
        if (!success) throw util::GameException();

        it2->second.location = &it2->first.location;
        if (it2->first.samplerDesc) it2->second.samplerDesc = &*it2->first.samplerDesc;

        return Texture(&it2->second);
    }

    bool ResourceManager::LRUCache::empty() {
        return head == nullptr;
    }

    void ResourceManager::LRUCache::insert(ReclaimNode* node) {
        if (head) {
            node->next = head;
            head->prev = node;
            head = node;
        } else {
            head = node;
            tail = node;
            node->next = nullptr;
            node->prev = nullptr;
        }
    }

    void ResourceManager::LRUCache::remove(ReclaimNode* node) {
        if (node->prev) {
            node->prev->next = node->next;
        } else {
            head = node->next;
        }
        if (node->next) {
            node->next->prev = node->prev;
        } else {
            tail = node->prev;
        }
    }

    bool ResourceManager::reclaimStep() {
        auto [candidateCache, candidate] = chooseReclaimCandidate();
        if (!candidateCache) return false;

        candidateCache->remove(candidate);
        reclaim(candidate);

        return true;
    }

    void ResourceManager::reclaim(ReclaimNode* node) {
        switch (node->value.kind) {
            case ReclaimKind::FontCPU: {
                FontResource* font = static_cast<FontResource*>(node->value.resource);
                evictFontCPU(*font);
                break;
            }
            case ReclaimKind::FontGPU: {
                FontResource* font = static_cast<FontResource*>(node->value.resource);
                evictFontGPU(*font);
                break;
            }
            case ReclaimKind::SoundCPU: {
                SoundResource* sound = static_cast<SoundResource*>(node->value.resource);
                evictSound(*sound);
                break;
            }
            case ReclaimKind::TextureCPU: {
                TextureResource* texture = static_cast<TextureResource*>(node->value.resource);
                evictTextureCPU(*texture);
                break;
            }
            case ReclaimKind::TextureGPU: {
                TextureResource* texture = static_cast<TextureResource*>(node->value.resource);
                evictTextureGPU(*texture);
                break;
            }
        }
    }

    std::pair<ResourceManager::LRUCache*, ReclaimNode*> ResourceManager::chooseReclaimCandidate() {
        double cpuPressure = mCPUMemoryProfile.memoryPressure();
        double gpuPressure = mGPUMemoryProfile.memoryPressure();

        bool shouldReclaimCPU = cpuPressure >= 0.80;
        bool shouldReclaimGPU = gpuPressure >= 0.80;

        if (!shouldReclaimCPU && !shouldReclaimGPU) return {nullptr, nullptr};

        if (shouldReclaimCPU && shouldReclaimGPU) {
            if (cpuPressure > gpuPressure) {
                auto candidate = chooseCPUReclaimCandidate();
                if (candidate.first) return candidate;
            }
            auto candidate = chooseGPUReclaimCandidate();
            if (candidate.first) return candidate;
        }

        if (shouldReclaimCPU) {
            auto candidate = chooseCPUReclaimCandidate();
            if (candidate.first) return candidate;
        }
        return chooseGPUReclaimCandidate();
    }

    std::pair<ResourceManager::LRUCache*, ReclaimNode*> ResourceManager::chooseCPUReclaimCandidate() {
        if (!mCPUReclaimCache.empty()) return {&mCPUReclaimCache, mCPUReclaimCache.head};

        return {nullptr, nullptr};
    }

    std::pair<ResourceManager::LRUCache*, ReclaimNode*> ResourceManager::chooseGPUReclaimCandidate() {
        if (!mGPUReclaimCache.empty()) return {&mGPUReclaimCache, mGPUReclaimCache.head};

        return {nullptr, nullptr};
    }

    backend::SamplerHandle ResourceManager::getSampler(SamplerDescriptor desc) {
        auto it = mSamplers.find(desc);
        if (it != mSamplers.end()) return it->second;

        backend::SamplerHandle sampler = mBackend.gpu.createSampler(desc.filter, desc.wrap);
        auto [it2, success] = mSamplers.emplace(desc, sampler);
        if (!success) throw util::GameException();

        return sampler;
    }

    void ResourceManager::markUsed(FontResource& resource) {
        if (resource.font) {
            size_t size = resource.font->getSizeBytes();
            mCPUMemoryProfile.potentialReclaimable -= size;
            mCPUReclaimCache.remove(&resource.cpuReclaimNode);
        }
        if (resource.textureHandle) {
            size_t size = resource.estimatedTextureSize;
            mGPUMemoryProfile.potentialReclaimable -= size;
            mGPUReclaimCache.remove(&resource.gpuReclaimNode);
        }
    }

    void ResourceManager::markUnused(FontResource& resource) {
        if (resource.font) {
            size_t size = resource.font->getSizeBytes();
            mCPUMemoryProfile.potentialReclaimable += size;
            resource.cpuReclaimNode.value.size = size;
            mCPUReclaimCache.insert(&resource.cpuReclaimNode);
        }
        if (resource.textureHandle) {
            size_t size = resource.estimatedTextureSize;
            mGPUMemoryProfile.potentialReclaimable += size;
            resource.gpuReclaimNode.value.size = size;
            mGPUReclaimCache.insert(&resource.gpuReclaimNode);
        }
    }

    void ResourceManager::markUsed(SoundResource& resource) {
        if (resource.audio) {
            size_t size = resource.audio->getSizeBytes();
            mCPUMemoryProfile.potentialReclaimable -= size;
            mCPUReclaimCache.remove(&resource.cpuReclaimNode);
        }
    }

    void ResourceManager::markUnused(SoundResource& resource) {
        if (resource.audio) {
            size_t size = resource.audio->getSizeBytes();
            mCPUMemoryProfile.potentialReclaimable += size;
            resource.cpuReclaimNode.value.size = size;
            mCPUReclaimCache.insert(&resource.cpuReclaimNode);
        }
    }

    void ResourceManager::markUsed(TextureResource& resource) {
        if (resource.image) {
            size_t size = resource.image->getSizeBytes();
            mCPUMemoryProfile.potentialReclaimable -= size;
            mCPUReclaimCache.remove(&resource.cpuReclaimNode);
        }
        if (resource.textureHandle) {
            size_t size = resource.estimatedTextureSize;
            mGPUMemoryProfile.potentialReclaimable -= size;
            mGPUReclaimCache.remove(&resource.gpuReclaimNode);
        }
    }

    void ResourceManager::markUnused(TextureResource& resource) {
        if (resource.image) {
            size_t size = resource.image->getSizeBytes();
            mCPUMemoryProfile.potentialReclaimable += size;
            resource.cpuReclaimNode.value.size = size;
            mCPUReclaimCache.insert(&resource.cpuReclaimNode);
        }
        if (resource.textureHandle) {
            size_t size = resource.estimatedTextureSize;
            mGPUMemoryProfile.potentialReclaimable += size;
            resource.gpuReclaimNode.value.size = size;
            mGPUReclaimCache.insert(&resource.gpuReclaimNode);
        }
    }

    void ResourceManager::realizeFontCPU(FontResource& resource) {
        resource.font = mBackend.assetProvider.loadFont(resource.location->cstr(), resource.fontSize, resource.codepoints, resource.codePointCount);
        mCPUMemoryProfile.used += resource.font->getSizeBytes();
    }

    void ResourceManager::realizeFontGPU(FontResource& resource) {
        if (!resource.font) realizeFontCPU(resource); // both the cpu and gpu sides of the font are needed to draw with it, so this is better

        resource.textureHandle = mBackend.gpu.createTexture(resource.font->atlas);
        resource.samplerHandle = resource.samplerDesc ? getSampler(*resource.samplerDesc) : nullptr;
        resource.estimatedTextureSize = resource.font->atlas.getSizeBytes();

        mGPUMemoryProfile.used += resource.estimatedTextureSize;
    }

    void ResourceManager::evictFontCPU(FontResource& resource) {
        mCPUMemoryProfile.used -= resource.font->getSizeBytes();
        if (resource.strongReferences == 0) mCPUMemoryProfile.potentialReclaimable -= resource.font->getSizeBytes();

        mBackend.assetProvider.unloadFont(*resource.font);
        resource.font = std::nullopt;
    }

    void ResourceManager::evictFontGPU(FontResource& resource) {
        mGPUMemoryProfile.used -= resource.estimatedTextureSize;
        if (resource.strongReferences == 0) mGPUMemoryProfile.potentialReclaimable -= resource.estimatedTextureSize;

        mBackend.gpu.destroyTexture(resource.textureHandle);
        resource.textureHandle = nullptr;
        resource.estimatedTextureSize = 0;
    }

    void ResourceManager::realizeSound(SoundResource& resource) {
        resource.audio = mBackend.assetProvider.loadAudio(resource.location->cstr());
        mCPUMemoryProfile.used += resource.audio->getSizeBytes();
    }

    void ResourceManager::evictSound(SoundResource& resource) {
        mCPUMemoryProfile.used -= resource.audio->getSizeBytes();
        if (resource.strongReferences == 0) mCPUMemoryProfile.potentialReclaimable -= resource.audio->getSizeBytes();

        mBackend.assetProvider.unloadAudio(*resource.audio);
        resource.audio = std::nullopt;
    }

    void ResourceManager::realizeTextureCPU(TextureResource& resource) {
        resource.image = mBackend.assetProvider.loadImage(resource.location->cstr());
        mCPUMemoryProfile.used += resource.image->getSizeBytes();
    }

    void ResourceManager::realizeTextureGPU(TextureResource& resource) {
        backend::Image image;
        bool cachedImage;
        if (!resource.image) {
            image = mBackend.assetProvider.loadImage(resource.location->cstr());
            cachedImage = false;
        } else {
            image = *resource.image;
            cachedImage = true;
        }

        size_t imageSizeBytes = image.getSizeBytes();

        if (!cachedImage && mCPUMemoryProfile.memoryPressureAfterAllocation(imageSizeBytes) < 0.7) {
            resource.image = image;
            mCPUMemoryProfile.used += imageSizeBytes;
            cachedImage = true;
        }

        resource.textureHandle = mBackend.gpu.createTexture(image);
        resource.samplerHandle = resource.samplerDesc ? getSampler(*resource.samplerDesc) : nullptr;
        resource.estimatedTextureSize = imageSizeBytes;

        if (!cachedImage) {
            mBackend.assetProvider.unloadImage(image);
        }

        mGPUMemoryProfile.used += resource.estimatedTextureSize;
    }

    void ResourceManager::evictTextureCPU(TextureResource& resource) {
        mCPUMemoryProfile.used -= resource.image->getSizeBytes();
        if (resource.strongReferences == 0) mCPUMemoryProfile.potentialReclaimable -= resource.image->getSizeBytes();

        mBackend.assetProvider.unloadImage(*resource.image);
        resource.image = std::nullopt;
    }

    void ResourceManager::evictTextureGPU(TextureResource& resource) {
        mGPUMemoryProfile.used -= resource.estimatedTextureSize;
        if (resource.strongReferences == 0) mGPUMemoryProfile.potentialReclaimable -= resource.estimatedTextureSize;

        mBackend.gpu.destroyTexture(resource.textureHandle);
        resource.textureHandle = nullptr;
        resource.estimatedTextureSize = 0; // safety to catch bugs from using texture size after evict
    }
}
