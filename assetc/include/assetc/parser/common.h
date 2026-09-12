// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_ASSETC_PARSER_COMMON_H
#define ASTERACITY_ASSETC_PARSER_COMMON_H

#include "assetc/parser/token_stream.h"

#include <file_formats/common/resource.h>

#include <cstdint>

namespace assetc {
    int ParseIntegerExpression(TokenStream& tokens);
    int64_t ParseInteger64Expression(TokenStream& tokens);
    float ParseFloatExpression(TokenStream& tokens);

    formats::Resource ParseResource(TokenStream& tokens);
}

#endif //ASTERACITY_ASSETC_PARSER_COMMON_H
