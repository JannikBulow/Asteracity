// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_FILE_FORMATS_COMMON_RESOURCE_H
#define ASTERACITY_FILE_FORMATS_COMMON_RESOURCE_H

#include "file_formats/binary/binary_reader.h"
#include "file_formats/binary/binary_writer.h"

namespace formats {
    struct Resource {
        std::string domain;
        std::string path;

        template<BinaryOutput Out>
        void serialize(BinaryWriter<Out>& writer) const {
            writer.writeString(domain);
            writer.writeString(path);
        }

        template<BinaryInput In>
        static Resource deserialize(BinaryReader<In>& reader) {
            std::string domain = reader.readString();
            std::string path = reader.readString();
            return {
                .domain = std::move(domain),
                .path = std::move(path)
            };
        }
    };
}

#endif //ASTERACITY_FILE_FORMATS_COMMON_RESOURCE_H
