// Copyright 2026 Jannik Laugmand Bülow

#include "assetc/lexer/lexer.h"

#include "assetc/parser/sprite_parser.h"

#include <file_formats/common/asset_header.h>

#include <format>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

formats::AssetType GetAssetType(std::string_view in) {
    if (in.ends_with(".source")) in.remove_suffix(7);

    size_t lastDot = in.find_last_of('.');
    if (lastDot == std::string::npos) throw util::AssetcException("unable to determine format");

    std::string_view fileType = in.substr(lastDot + 1);

    if (fileType == "sprite") {
        return formats::SPRITE;
    }

    throw util::AssetcException("unknown format");
}

int main(int argc, char** argv) {
    std::unordered_map<std::string, std::string> args;
    if (argc > 1) {
        args.reserve((argc - 1) / 2);

        for (int i = 1; i < argc; i++) {
            std::string_view arg = argv[i];

            if (!arg.starts_with("--")) throw util::AssetcException(std::format("missing -- at {}", arg));

            std::string key(arg.substr(2));
            std::string_view value;

            if (i == argc - 1) value = "";
            else value = argv[++i];

            auto it = args.find(key);
            if (it != args.end()) {
                it->second.reserve(it->second.size() + value.size() + 1);
                it->second += ',';
                it->second += value;
            } else {
                args.emplace(key, std::string(value));
            }
        }
    }

    std::string& in = args.at("in");
    std::string& out = args.at("out");

    std::ifstream inputFile(in);
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    buffer << '\n'; // vlex is retarded
    inputFile.close();

    std::string text = buffer.str();

    assetc::Lexer lexer(text, in);
    assetc::TokenStream tokens(lexer.lex());

    formats::AssetType type = GetAssetType(in);

    formats::BinaryWriter<formats::FileOutput> writer(fopen(out.c_str(), "wb"));

    switch (type) {
        case formats::SPRITE: {
            assetc::SpriteParser parser(tokens);
            formats::Sprite sprite = parser.parse();
            writer.write(sprite);
            break;
        }
    }

    return 0;
}
