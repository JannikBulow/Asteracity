// Copyright 2026 Jannik Laugmand Bülow

#include "engine/util/exceptions.h"
#include "engine/util/resource_location.h"

#include <linux/limits.h>

#include <cstring>
#include <filesystem>
#include <unistd.h>

namespace util {
    const char* ResourceLocation::DefaultDomain = "asteracity";

    ResourceLocation::ResourceLocation(std::string_view resource)
        : mPath(CreatePath(DefaultDomain, resource)) {}

    ResourceLocation::ResourceLocation(std::string_view domain, std::string_view resource)
        : mPath(CreatePath(domain, resource)) {}

    bool ResourceLocation::operator==(const ResourceLocation& other) const {
        return strcmp(mPath.get(), other.mPath.get()) == 0;
    }

    const char* ResourceLocation::cstr() const {
        return mPath.get();
    }

    std::unique_ptr<const char[]> ResourceLocation::CreatePath(std::string_view domain, std::string_view resource) {
        char buf[PATH_MAX];
        if (getcwd(buf, sizeof(buf)) == nullptr) throw GameException();

        std::string basePath = buf;
        if (basePath.ends_with('/')) basePath += "assets/";
        else basePath += "/assets/";

        int needSep = domain.ends_with(std::filesystem::path::preferred_separator) ? 0 : 1;
        size_t size = basePath.size() + domain.size() + needSep + resource.size();
        auto path = std::make_unique<char[]>(size);

        memcpy(path.get(), basePath.data(), basePath.size());
        memcpy(path.get() + basePath.size(), domain.data(), domain.size());
        if (needSep) path[basePath.size() + domain.size()] = std::filesystem::path::preferred_separator;
        memcpy(path.get() + basePath.size() + domain.size() + needSep, resource.data(), resource.size());
        path[size] = '\0';

        return path;
    }
}
