// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_UTIL_UNICODE_H
#define ASTERACITY_ENGINE_UTIL_UNICODE_H

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>

namespace unicode {
    using codepoint = uint32_t;
    using string = std::basic_string<codepoint>;

    constexpr std::optional<char> ToAscii(codepoint cp) {
        if (cp > 0x7F) return std::nullopt;
        return static_cast<char>(cp);
    }

    codepoint GetNextCodepoint(const char* text, int* codepointSize);

    size_t Hash(const codepoint* codepoints, size_t codepointCount);
}

#endif //ASTERACITY_ENGINE_UTIL_UNICODE_H
