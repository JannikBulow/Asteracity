// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_FILE_FORMATS_COMMON_SIZE_H
#define ASTERACITY_FILE_FORMATS_COMMON_SIZE_H

#include "file_formats/binary/binary_reader.h"
#include "file_formats/binary/binary_writer.h"

namespace formats {
    struct IntSize {
        int width;
        int height;

        template<BinaryOutput Out>
        void serialize(BinaryWriter<Out>& writer) const {
            writer.writeI32(width);
            writer.writeI32(height);
        }

        template<BinaryInput In>
        static IntSize deserialize(BinaryReader<In>& reader) {
            int width = reader.readI32();
            int height = reader.readI32();
            return {
                .width = width,
                .height = height
            };
        }
    };

    struct FloatSize {
        float width;
        float height;

        template<BinaryOutput Out>
        void serialize(BinaryWriter<Out>& writer) const {
            writer.writeF32(width);
            writer.writeF32(height);
        }

        template<BinaryInput In>
        static FloatSize deserialize(BinaryReader<In>& reader) {
            float width = reader.readFloat();
            float height = reader.readFloat();
            return {
                .width = width,
                .height = height
            };
        }
    };
}

#endif //ASTERACITY_FILE_FORMATS_COMMON_SIZE_H
