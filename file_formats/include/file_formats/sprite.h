// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_FILE_FORMATS_SPRITE_H
#define ASTERACITY_FILE_FORMATS_SPRITE_H

#include "file_formats/common/asset_header.h"
#include "file_formats/common/resource.h"

namespace formats {
    struct Sprite {
        AssetHeader header;
        Resource texture; // or sheet, but a sheet is just a texture with metadata
        int width;
        int height;
        float uvLeft;
        float uvRight;
        float uvTop;
        float uvBottom;

        template<BinaryOutput Out>
        void serialize(BinaryWriter<Out>& writer) const {
            writer.write(header);
            writer.write(texture);
            writer.writeU32(width);
            writer.writeU32(height);
            writer.writeFloat(uvLeft);
            writer.writeFloat(uvRight);
            writer.writeFloat(uvTop);
            writer.writeFloat(uvBottom);
        }

        template<BinaryInput In>
        static Sprite deserialize(BinaryReader<In>& reader) {
            AssetHeader header = reader.template read<AssetHeader>();
            Resource texture = reader.template read<Resource>();
            int width = reader.readU32();
            int height = reader.readU32();
            float uvLeft = reader.readFloat();
            float uvRight = reader.readFloat();
            float uvTop = reader.readFloat();
            float uvBottom = reader.readFloat();
            return {
                .header = std::move(header),
                .texture = std::move(texture),
                .width = width,
                .height = height,
                .uvLeft = uvLeft,
                .uvRight = uvRight,
                .uvTop = uvTop,
                .uvBottom = uvBottom,
            };
        }
    };
}

#endif //ASTERACITY_FILE_FORMATS_SPRITE_H
