// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_FILE_FORMATS_SPRITE_H
#define ASTERACITY_FILE_FORMATS_SPRITE_H

#include "file_formats/common/asset_header.h"
#include "file_formats/common/color.h"
#include "file_formats/common/rect.h"
#include "file_formats/common/resource.h"
#include "file_formats/common/size.h"

namespace formats {
    struct Sprite {
        AssetHeader header;
        Resource texture; // or sheet, but a sheet is just a texture with metadata
        IntSize size;
        FloatRect uv;
        Color tint{255, 255, 255, 255};

        template<BinaryOutput Out>
        void serialize(BinaryWriter<Out>& writer) const {
            writer.write(header);
            writer.write(texture);
            writer.write(size);
            writer.write(uv);
            writer.write(tint);
        }

        template<BinaryInput In>
        static Sprite deserialize(BinaryReader<In>& reader) {
            AssetHeader header = reader.template read<AssetHeader>();
            Resource texture = reader.template read<Resource>();
            IntSize size = reader.template read<IntSize>();
            FloatRect uv = reader.template read<FloatRect>();
            Color tint = reader.template read<Color>();
            return {
                .header = std::move(header),
                .texture = std::move(texture),
                .size = size,
                .uv = uv,
                .tint = tint
            };
        }
    };
}

#endif //ASTERACITY_FILE_FORMATS_SPRITE_H
