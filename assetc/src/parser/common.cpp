// Copyright 2026 Jannik Laugmand Bülow

#include "assetc/parser/common.h"

#include <charconv>
#include <cmath>

namespace assetc {
    template<class T>
    requires(std::is_integral_v<T> || std::is_floating_point_v<T>)
    class ExpressionParser {
    public:
        explicit ExpressionParser(TokenStream& tokens)
            : mTokens(tokens) {}

        T parse() {
            return parseAdditive();
        }

    private:
        TokenStream& mTokens;

        auto& current() const { return mTokens.current(); }
        auto& consume() const { return mTokens.consume(); }
        auto& peek(int offset) const { return mTokens.peek(offset); }
        void expectToken(TokenType type) const { mTokens.expectToken(type); }

        T parseAdditive() {
            T value = parseMultiplicative();

            while (true) {
                switch (current().getTokenType()) {
                    case TokenType::Plus:
                        consume();
                        value += parseMultiplicative();
                        break;
                    case TokenType::Minus:
                        consume();
                        value -= parseMultiplicative();
                        break;
                    default:
                        return value;
                }
            }
        }

        T parseMultiplicative() {
            T value = parseUnary();

            while (true) {
                switch (current().getTokenType()) {
                    case TokenType::Star:
                        consume();
                        value *= parseUnary();
                        break;
                    case TokenType::Slash: {
                        consume();

                        T divisor = parseUnary();
                        if (divisor == T(0)) throw util::AssetcException("division by zero");

                        value /= divisor;
                        break;
                    }
                    case TokenType::Percent: {
                        consume();

                        T divisor = parseUnary();
                        if (divisor == T(0)) throw util::AssetcException("modulo by zero");

                        if constexpr (std::is_integral_v<T>) {
                            value %= divisor;
                        } else if constexpr (std::is_floating_point_v<T>) {
                            value = std::fmod(value, divisor);
                        }
                        break;
                    }
                    default:
                        return value;
                }
            }
        }

        T parseUnary() {
            switch (current().getTokenType()) {
                case TokenType::Plus:
                    consume();
                    return parseUnary();

                case TokenType::Minus:
                    consume();
                    return -parseUnary();

                default:
                    return parsePrimary();
            }
        }

        T parsePrimary() {
            if (current().getTokenType() == TokenType::LeftParen) {
                consume();

                T value = parseAdditive();

                expectToken(TokenType::RightParen);
                consume();

                return value;
            }

            if constexpr (std::is_integral_v<T>) {
                expectToken(TokenType::IntegerLiteral);

                std::string_view text = consume().getText();
                T value = 0;
                auto [ptr, error] = std::from_chars(text.data(), text.data() + text.size(), value);

                if (error != std::errc() || ptr != text.data() + text.size()) {
                    throw util::AssetcException("invalid integer");
                }

                return value;
            } else if constexpr (std::is_floating_point_v<T>) {
                expectToken(TokenType::FloatLiteral);

                std::string_view text = consume().getText();
                T value = 0;
                auto [ptr, error] = std::from_chars(text.data(), text.data() + text.size(), value);
                if (error != std::errc() || ptr != text.data() + text.size()) {
                    throw util::AssetcException("invalid floating point");
                }

                return value;
            }
        }
    };

    int ParseIntegerExpression(TokenStream& tokens) {
        return ExpressionParser<int>(tokens).parse();
    }

    int64_t ParseInteger64Expression(TokenStream& tokens) {
        return ExpressionParser<int64_t>(tokens).parse();
    }

    float ParseFloatExpression(TokenStream& tokens) {
        return ExpressionParser<float>(tokens).parse();
    }

    formats::Resource ParseResource(TokenStream& tokens) {
        tokens.consume(); // resource keyword

        tokens.expectToken(TokenType::LeftParen);
        tokens.consume();

        std::string domain;
        std::string path;

        tokens.expectToken(TokenType::StringLiteral);
        path = tokens.consume().getText();

        if (tokens.current().getTokenType() == TokenType::Comma) {
            tokens.consume();
            domain = std::move(path);
            path = tokens.consume().getText();
        }

        tokens.expectToken(TokenType::RightParen);
        tokens.consume();

        return {
            .domain = std::move(domain),
            .path = std::move(path)
        };
    }
}
