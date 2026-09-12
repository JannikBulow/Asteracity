// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ASSETC_PARSER_TOKEN_STREAM_H
#define ASTERACITY_ASSETC_PARSER_TOKEN_STREAM_H

#include "assetc/lexer/token.h"

#include "assetc/util/exceptions.h"

#include <vector>

namespace assetc {
    using namespace lexer;

    class TokenStream {
    public:
        TokenStream(std::vector<Token> tokens)
            : mTokens(std::move(tokens))
            , mCurrentIndex(0) {}

        const Token& current() const { return mTokens[mCurrentIndex]; }
        const Token& consume() { return mTokens[mCurrentIndex++]; }
        const Token& peek(int offset) const { return mTokens[mCurrentIndex + offset]; }

        void expectToken(TokenType type) const {
            if (current().getTokenType() != type) {
                throw util::AssetcException("expectTokenType");
            }
        }

    private:
        std::vector<Token> mTokens;
        size_t mCurrentIndex;
    };
}

#endif //ASTERACITY_ASSETC_PARSER_TOKEN_STREAM_H
