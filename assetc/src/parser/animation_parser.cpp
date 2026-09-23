// Copyright 2026 Jannik Laugmand Bülow

#include "assetc/parser/animation_parser.h"

#include "assetc/parser/common.h"

namespace assetc {
    AnimationParser::AnimationParser(TokenStream& tokens)
        : mTokens(tokens) {}

    formats::Animation AnimationParser::parse() {
        mProgress.header.type = formats::ANIMATION;
        mProgress.type = formats::Animation::Type::FrameByFrame;

        while (current().getTokenType() != TokenType::EndOfFile) {
            parseCommand();
        }

        return std::move(mProgress);
    }

    void AnimationParser::parseCommand() {
        switch (current().getTokenType()) {
            case TokenType::AnimationKeyword:
                parseAnimationCommand();
                break;
            case TokenType::VersionKeyword:
                parseVersionCommand();
                break;
            case TokenType::FrameKeyword:
                parseFrameCommand();
                break;§
            case TokenType::FromKeyword:
                parseFromCommand();
                break;
            case TokenType::SizeKeyword:
                parseSizeCommand();
                break;

            default:
                throw util::AssetcException("weird command " + std::string(current().getText()));
        }
    }

    void AnimationParser::parseAnimationCommand() {
        consume();
        expectToken(TokenType::StringLiteral);
        mProgress.header.name = consume().getText();
    }

    void AnimationParser::parseVersionCommand() {
        consume();
        mProgress.header.version = ParseIntegerExpression(mTokens);
    }

    void AnimationParser::parseFrameCommand() {
        consume();
        formats::AnimationFrame frame;
        frame.sprite = ParseResource(mTokens);
        frame.duration = ParseFloatExpression(mTokens);
    }

    void AnimationParser::parseFromCommand() {
        consume();
        mProgress.type = formats::Animation::Type::Generated;
        mProgress.texture = ParseResource(mTokens);
        mProgress.rows = ParseIntegerExpression(mTokens);
        mProgress.columns = ParseIntegerExpression(mTokens);
        mProgress.frameDuration = ParseFloatExpression(mTokens);
    }

    void AnimationParser::parseSizeCommand() {
        consume(); // size
        mProgress.frameSize = ParseFloatSize(mTokens);
    }
}
