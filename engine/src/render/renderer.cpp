// Copyright 2026 Jannik Laugmand Bülow

#include "engine/render/renderer.h"

namespace engine {
    Renderer::Renderer(backend::Backend& backend)
        : mBackend(backend) {}

    math::Vec2 Renderer::measureText(const std::string& text, float fontSize) {
        if (!mDefaultFont) return math::Vec2::Zero();
        return measureText(*mDefaultFont, text, fontSize);
    }

    math::Vec2 Renderer::measureText(const std::string& text, float fontSize, float spacing, float textLineSpacing) {
        if (!mDefaultFont) return math::Vec2::Zero();
        return measureText(*mDefaultFont, text, fontSize, spacing, textLineSpacing);
    }

    math::Vec2 Renderer::measureText(const Font& font, const std::string& text, float fontSize) {
        return measureText(font, text, fontSize, fontSize / 12, 2.0f);
    }

    math::Vec2 Renderer::measureText(const Font& font, const std::string& text, float fontSize, float spacing, float textLineSpacing) {
        return font.font().measureText(text.c_str(), fontSize, spacing, textLineSpacing);
    }

    void Renderer::clear(math::Color color) {
        mBackend.renderer.clearScreen(color);
    }

    void Renderer::drawRect(math::Vec2 position, math::Vec2 size, math::Color color, float rotation) {
        mBackend.renderer.drawRect({
            .position = position,
            .size = size,
            .color = color,
            .rotation = rotation
        });
    }

    void Renderer::drawRect(math::Rect rect, math::Color color, float rotation) {
        drawRect(rect.center(), rect.size(), color, rotation);
    }

    void Renderer::drawRectOutline(math::Vec2 position, math::Vec2 size, float thickness, math::Color color, float rotation) {
        math::Vec2 halfSize = size / 2.0f;
        drawRectOutline({position.x - halfSize.x, position.x + halfSize.x, position.y + halfSize.y, position.y - halfSize.y}, thickness, color, rotation);
    }

    void Renderer::drawRectOutline(math::Rect rect, float thickness, math::Color color, float rotation) {
        if (rotation != 0) throw util::WIPException();

        if (thickness > 0.0f) {
            thickness = std::min(thickness, std::min(rect.width(), rect.height()) / 2.0f);
        }

        if (thickness > 0.0f) {
            math::Rect top = {rect.left, rect.right, rect.top, rect.top - thickness};
            math::Rect bottom = {rect.left, rect.right, rect.bottom + thickness, rect.bottom};
            math::Rect left = {rect.left, rect.left + thickness, rect.top - thickness, rect.bottom + thickness};
            math::Rect right = {rect.right - thickness, rect.right, rect.top - thickness, rect.bottom + thickness};

            drawRect(top, color);
            drawRect(bottom, color);
            drawRect(left, color);
            drawRect(right, color);
        } else {
            thickness *= -1.0f;

            math::Rect top = {rect.left - thickness, rect.right + thickness, rect.top + thickness, rect.top};
            math::Rect bottom = {rect.left - thickness, rect.right + thickness, rect.bottom, rect.bottom - thickness};
            math::Rect left = {rect.left - thickness, rect.left, rect.top, rect.bottom};
            math::Rect right = {rect.right, rect.right + thickness, rect.top, rect.bottom};

            drawRect(top, color);
            drawRect(bottom, color);
            drawRect(left, color);
            drawRect(right, color);
        }
    }

    void Renderer::drawTexture(const Texture& texture, math::Vec2 position, math::Vec2 size, math::Color color, float rotation, math::Rect uv) {
        mBackend.renderer.drawTexture({
            .texture = texture.handle(),
            .sampler = texture.sampler(),
            .position = position,
            .size = size,
            .uv = uv,
            .color = color,
            .rotation = rotation
        });
    }

    void Renderer::drawSprite(const Sprite& sprite, math::Vec2 position, float rotation) {
        drawTexture(sprite.texture(), position, sprite.size(), sprite.tint(), rotation, sprite.uv());
    }

    void Renderer::drawText(const std::string& text, math::Vec2 position, float fontSize, math::Color color, bool centerOrigin) {
        if (!mDefaultFont) return;
        drawText(*mDefaultFont, text, position, fontSize, color, centerOrigin);
    }

    void Renderer::drawText(const std::string& text, math::Vec2 position, float fontSize, float spacing, float textLineSpacing, math::Color color, bool centerOrigin) {
        if (!mDefaultFont) return;
        drawText(*mDefaultFont, text, position, fontSize, spacing, textLineSpacing, color, centerOrigin);
    }

    void Renderer::drawText(const Font& font, const std::string& text, math::Vec2 position, float fontSize, math::Color color, bool centerOrigin) {
        float spacing = fontSize / 12;
        drawText(font, text, position, fontSize, spacing, 2.0f, color, centerOrigin);
    }

    void Renderer::drawText(const Font& font, const std::string& text, math::Vec2 position, float fontSize, float spacing, float textLineSpacing, math::Color color, bool centerOrigin) {
        mBackend.renderer.drawText({
            .texture = font.texture(),
            .sampler = font.sampler(),
            .font = font.font(),
            .text = text.c_str(),
            .textLength = text.length(),
            .fontSize = fontSize,
            .spacing = spacing,
            .textLineSpacing = textLineSpacing,
            .position = position,
            .color = color,
            .centerOrigin = centerOrigin
        });
    }
}
