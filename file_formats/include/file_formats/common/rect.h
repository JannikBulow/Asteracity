// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_FILE_FORMATS_COMMON_RECT_H
#define ASTERACITY_FILE_FORMATS_COMMON_RECT_H

#include "file_formats/binary/binary_reader.h"
#include "file_formats/binary/binary_writer.h"

namespace formats {
    struct IntRect {
        int left;
        int right;
        int top;
        int bottom;

        template<BinaryOutput Out>
        void serialize(BinaryWriter<Out>& writer) const {
            writer.writeI32(left);
            writer.writeI32(right);
            writer.writeI32(top);
            writer.writeI32(bottom);
        }

        template<BinaryInput In>
        static IntRect deserialize(BinaryReader<In>& reader) {
            int left = reader.readI32();
            int right = reader.readI32();
            int top = reader.readI32();
            int bottom = reader.readI32();
            return {
                .left = left,
                .right = right,
                .top = top,
                .bottom = bottom
            };
        }
    };

    struct FloatRect {
        float left;
        float right;
        float top;
        float bottom;

        template<BinaryOutput Out>
        void serialize(BinaryWriter<Out>& writer) const {
            writer.writeFloat(left);
            writer.writeFloat(right);
            writer.writeFloat(top);
            writer.writeFloat(bottom);
        }

        template<BinaryInput In>
        static FloatRect deserialize(BinaryReader<In>& reader) {
            float left = reader.readFloat();
            float right = reader.readFloat();
            float top = reader.readFloat();
            float bottom = reader.readFloat();
            return {
                .left = left,
                .right = right,
                .top = top,
                .bottom = bottom
            };
        }
    };
}

#endif //ASTERACITY_FILE_FORMATS_COMMON_RECT_H
