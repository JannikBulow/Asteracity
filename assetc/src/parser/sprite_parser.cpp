// Copyright 2026 Jannik Laugmand Bülow

#include "assetc/parser/common.h"
#include "assetc/parser/sprite_parser.h"

namespace assetc {
    SpriteParser::SpriteParser(TokenStream& tokens)
        : mTokens(tokens) {}

    formats::Sprite SpriteParser::parse() {
        mProgress.header.type = formats::SPRITE;

        while (current().getTokenType() != TokenType::EndOfFile) {
            parseCommand();
        }

        return std::move(mProgress);
    }

    void SpriteParser::parseCommand() {
        switch (current().getTokenType()) {
            case TokenType::SpriteKeyword:
                parseSpriteCommand();
                break;
            case TokenType::VersionKeyword:
                parseVersionCommand();
                break;
            case TokenType::TextureKeyword:
            case TokenType::SheetKeyword:
                parseTextureOrSheetCommand();
                break;
            case TokenType::SizeKeyword:
                parseSizeCommand();
                break;
            case TokenType::UvKeyword:
                parseUVCommand();
                break;

            default:
                throw util::AssetcException("weird command " + std::string(current().getText()));
        }
    }

    void SpriteParser::parseSpriteCommand() {
        consume(); // sprite
        expectToken(TokenType::StringLiteral);
        mProgress.header.name = consume().getText();
    }

    void SpriteParser::parseVersionCommand() {
        consume(); // version
        mProgress.header.version = ParseIntegerExpression(mTokens);
    }

    void SpriteParser::parseTextureOrSheetCommand() {
        consume(); // texture or sheet
        mProgress.texture = ParseResource(mTokens);
    }

    void SpriteParser::parseSizeCommand() {
        consume(); // size
        mProgress.width = ParseIntegerExpression(mTokens);
        mProgress.height = ParseIntegerExpression(mTokens);
    }

    void SpriteParser::parseUVCommand() {
        consume(); // uv
        mProgress.uvLeft = ParseFloatExpression(mTokens);
        mProgress.uvRight = ParseFloatExpression(mTokens);
        mProgress.uvTop = ParseFloatExpression(mTokens);
        mProgress.uvBottom = ParseFloatExpression(mTokens);
    }
}
