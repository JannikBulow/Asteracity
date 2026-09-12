// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ASSETC_UTIL_EXCEPTIONS_H
#define ASTERACITY_ASSETC_UTIL_EXCEPTIONS_H

#include <stdexcept>

namespace util {
    class AssetcException : public std::runtime_error {
    public:
        explicit AssetcException(const char* message = "no message") : runtime_error(message) {}
        explicit AssetcException(const std::string& message) : runtime_error(message) {}
    };
}

#endif //ASTERACITY_ASSETC_UTIL_EXCEPTIONS_H
