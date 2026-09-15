// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ASSETC_PARSER_COMMON_H
#define ASTERACITY_ASSETC_PARSER_COMMON_H

#include "assetc/parser/token_stream.h"

#include <file_formats/common/rect.h>
#include <file_formats/common/resource.h>
#include <file_formats/common/size.h>

#include <cstdint>

namespace assetc {
    int ParseIntegerExpression(TokenStream& tokens);
    int64_t ParseInteger64Expression(TokenStream& tokens);
    float ParseFloatExpression(TokenStream& tokens);

    formats::IntRect ParseIntRect(TokenStream& tokens);
    formats::FloatRect ParseFloatRect(TokenStream& tokens);

    formats::Resource ParseResource(TokenStream& tokens);

    formats::IntSize ParseIntSize(TokenStream& tokens);
    formats::FloatSize ParseFloatSize(TokenStream& tokens);
}

#endif //ASTERACITY_ASSETC_PARSER_COMMON_H
