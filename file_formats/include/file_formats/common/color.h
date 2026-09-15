// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_FILE_FORMATS_COMMON_COLOR_H
#define ASTERACITY_FILE_FORMATS_COMMON_COLOR_H

#include "file_formats/binary/binary_reader.h"
#include "file_formats/binary/binary_writer.h"

namespace formats {
    struct Color {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        template<BinaryOutput Out>
        void serialize(BinaryWriter<Out>& writer) const {
            writer.writeU8(r);
            writer.writeU8(g);
            writer.writeU8(b);
            writer.writeU8(a);
        }

        template<BinaryInput In>
        static Color deserialize(BinaryReader<In>& reader) {
            uint8_t r = reader.readU8();
            uint8_t g = reader.readU8();
            uint8_t b = reader.readU8();
            uint8_t a = reader.readU8();
            return {
                .r = r,
                .g = g,
                .b = b,
                .a = a
            };
        }
    };
}

#endif //ASTERACITY_FILE_FORMATS_COMMON_COLOR_H
