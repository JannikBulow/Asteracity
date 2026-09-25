// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ENGINE_BACKEND_ASSET_PROVIDER_H
#define ASTERACITY_ENGINE_BACKEND_ASSET_PROVIDER_H

#include "engine/util/math.h"
#include "engine/util/unicode.h"

#include <cstdint>
#include <cstring>
#include <span>

namespace backend {
    enum class AudioSampleFormat {
        S8,
        S16,
        S32F
    };

    struct Audio {
        uint32_t sampleRate;
        uint32_t channels;
        uint64_t frameCount;
        float* samples;

        size_t getSizeBytes() const {
            return frameCount * channels * sizeof(float);
        }
    };

    enum class ImageFormat {
        R8,
        RGB8,
        RGBA8,
    };

    struct Image {
        int width;
        int height;
        ImageFormat format;
        uint8_t* pixels;

        int getBytesPerPixel() const {
            int bytesPerPixel = 4;
            switch (format) {
                case ImageFormat::R8:
                    bytesPerPixel = 1;
                    break;
                case ImageFormat::RGB8:
                    bytesPerPixel = 3;
                    break;
                case ImageFormat::RGBA8:
                    bytesPerPixel = 4;
                    break;
            }
            return bytesPerPixel;
        }

        size_t getSizeBytes() const {
            return width * height * getBytesPerPixel();
        }
    };

    struct Glyph {
        unicode::codepoint codepoint; // unicode
        float offsetX;
        float offsetY;
        float advanceX;
        math::Rect atlasBounds;
    };

    struct Font {
        int baseSize;
        int glyphCount;
        Image atlas;
        Glyph* glyphs;

        size_t getSizeBytes() const {
            return glyphCount * sizeof(Glyph) + atlas.getSizeBytes();
        }

        int getGlyphIndex(unicode::codepoint codepoint) const {
            int index = 0;
            int fallbackIndex = 0;

            for (int i = 0; i < glyphCount; i++) {
                if (glyphs[i].codepoint == '?') fallbackIndex = i;

                if (glyphs[i].codepoint == codepoint) {
                    index = i;
                    break;
                }
            }

            if (index == 0 && glyphs[0].codepoint != codepoint) index = fallbackIndex;
            return index;
        }

        math::Vec2 measureText(const char* text, float fontSize, float spacing = 1.0f, float textLineSpacing = 2.0f) const {
            if (!text || !text[0]) return math::Vec2::Zero();

            size_t textLength = strlen(text);
            size_t tempByteCounter = 0;
            size_t byteCounter = 0;

            float textWidth = 0.0f;
            float tempTextWidth = 0.0f;
            float textHeight = fontSize;

            //TODO: introduce world space measurement when the time is right. maybe move this to IRenderer
            float scale = fontSize / static_cast<float>(baseSize);

            unicode::codepoint codepoint = 0;
            int index = 0;

            for (size_t i = 0; i < textLength;) {
                int codepointSize = 0;
                unicode::codepoint codepoint =
                    unicode::GetNextCodepoint(text + i, &codepointSize);

                const Glyph& glyph = glyphs[getGlyphIndex(codepoint)];

                i += codepointSize;

                if (codepoint == '\n') {
                    if (textWidth > tempTextWidth)
                        tempTextWidth = textWidth;

                    textWidth = 0.0f;
                    textHeight += fontSize + textLineSpacing;
                    continue;
                }

                if (glyph.advanceX == 0) textWidth += glyph.atlasBounds.width() * scale;
                else textWidth += glyph.advanceX * scale;

                textWidth += spacing;
            }

            if (textWidth > tempTextWidth) tempTextWidth = textWidth;

            return {
                tempTextWidth,
                textHeight
            };
        }

        math::Vec2 measureCodepoints(std::span<const unicode::codepoint> codepoints, float fontSize, float spacing = 1.0f, float textLineSpacing = 2.0f) const {
            if (codepoints.empty()) return math::Vec2::Zero();

            float textWidth = 0.0f;
            float tempTextWidth = 0.0f;
            float textHeight = fontSize;

            float scale = fontSize / static_cast<float>(baseSize);

            for (unicode::codepoint codepoint : codepoints) {
                const Glyph& glyph = glyphs[getGlyphIndex(codepoint)];

                if (codepoint == '\n') {
                    if (textWidth > tempTextWidth)
                        tempTextWidth = textWidth;

                    textWidth = 0.0f;
                    textHeight += fontSize + textLineSpacing;
                    continue;
                }

                if (glyph.advanceX == 0) textWidth += glyph.atlasBounds.width() * scale;
                else textWidth += glyph.advanceX * scale;

                textWidth += spacing;
            }

            if (textWidth > tempTextWidth) tempTextWidth = textWidth;

            return {
                tempTextWidth,
                textHeight
            };
        }
    };

    class IAssetProvider {
    public:
        virtual ~IAssetProvider() = default;

        virtual Audio loadAudio(const char* path) = 0;
        virtual void unloadAudio(Audio audio) = 0;

        virtual Image loadImage(const char* path) = 0;
        virtual void unloadImage(Image image) = 0;

        virtual Font loadFont(const char* path, int size, const unicode::codepoint* codepoints = nullptr, int codepointCount = 0) = 0;
        virtual void unloadFont(Font font) = 0;
    };
}

#endif //ASTERACITY_ENGINE_BACKEND_ASSET_PROVIDER_H
