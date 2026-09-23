// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ASSETC_PARSER_ANIMATION_PARSER_H
#define ASTERACITY_ASSETC_PARSER_ANIMATION_PARSER_H

#include "assetc/parser/token_stream.h"

#include <file_formats/animation.h>

namespace assetc {
    class AnimationParser {
    public:
        explicit AnimationParser(TokenStream& tokens);

        formats::Animation parse();

    private:
        TokenStream& mTokens;

        formats::Animation mProgress{};

        auto& current() const { return mTokens.current(); }
        auto& consume() { return mTokens.consume(); }
        auto& peek(int offset) const { return mTokens.peek(offset); }
        void expectToken(TokenType type) const { mTokens.expectToken(type); }

        void parseCommand();

        void parseAnimationCommand();
        void parseVersionCommand();
        void parseFrameCommand();
        void parseFromCommand();
        void parseSizeCommand();
    };
}

#endif //ASTERACITY_ASSETC_PARSER_ANIMATION_PARSER_H
