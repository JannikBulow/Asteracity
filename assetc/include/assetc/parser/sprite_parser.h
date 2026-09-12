// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ASSETC_PARSER_SPRITE_PARSER_H
#define ASTERACITY_ASSETC_PARSER_SPRITE_PARSER_H

#include "assetc/parser/token_stream.h"

#include <file_formats/sprite.h>

namespace assetc {
    class SpriteParser {
    public:
        explicit SpriteParser(TokenStream& tokens);

        formats::Sprite parse();

    private:
        TokenStream& mTokens;

        formats::Sprite mProgress{};

        auto& current() const { return mTokens.current(); }
        auto& consume() { return mTokens.consume(); }
        auto& peek(int offset) const { return mTokens.peek(offset); }
        void expectToken(TokenType type) const { mTokens.expectToken(type); }

        void parseCommand();

        void parseSpriteCommand();
        void parseVersionCommand();
        void parseTextureOrSheetCommand();
        void parseSizeCommand();
        void parseUVCommand();
    };
}

#endif //ASTERACITY_ASSETC_PARSER_SPRITE_PARSER_H
