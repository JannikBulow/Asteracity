// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_FILE_FORMATS_COMMON_ASSET_HEADER_H
#define ASTERACITY_FILE_FORMATS_COMMON_ASSET_HEADER_H

#include "file_formats/binary/binary_reader.h"
#include "file_formats/binary/binary_writer.h"

namespace formats {
    enum AssetType : uint16_t {
        SPRITE = 0
    };

    struct AssetHeader {
        uint32_t magic;
        uint16_t version;
        uint16_t type;
        std::string name;

        template<BinaryOutput Out>
        void serialize(BinaryWriter<Out>& writer) const {
            writer.writeU32(magic);
            writer.writeU16(version);
            writer.writeU16(type);
            writer.writeString(name);
        }

        template<BinaryInput In>
        static AssetHeader deserialize(BinaryReader<In>& reader) {
            uint32_t magic = reader.readU32();
            uint16_t version = reader.readU16();
            uint16_t type = reader.readU16();
            std::string name = reader.readString();
            return {
                .magic = magic,
                .version = version,
                .type = type,
                .name = std::move(name)
            };
        }
    };
}

#endif //ASTERACITY_FILE_FORMATS_COMMON_ASSET_HEADER_H
