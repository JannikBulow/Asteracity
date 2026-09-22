// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_FILE_FORMATS_ANIMATION_H
#define ASTERACITY_FILE_FORMATS_ANIMATION_H

#include <chrono>

#include "file_formats/common/asset_header.h"
#include "file_formats/common/color.h"
#include "file_formats/common/resource.h"
#include "file_formats/common/size.h"

namespace formats {
    struct AnimationFrame {
        Resource sprite;
        float duration;

        template<BinaryOutput Out>
        void serialize(BinaryWriter<Out>& writer) const {
            writer.write(sprite);
            writer.writeFloat(duration);
        }

        template<BinaryInput In>
        static AnimationFrame deserialize(BinaryReader<In>& reader) {
            Resource sprite = reader.template read<Resource>();
            float duration = reader.readFloat();
            return {
                .sprite = sprite,
                .duration = duration
            };
        }
    };

    struct Animation {
        enum class Type : uint8_t {
            FrameByFrame = 0,
            Generated = 1
        };

        AssetHeader header;
        Type type;

        // FrameByFrame
        std::vector<AnimationFrame> frames;

        // Generated
        Resource texture;
        int rows;
        int columns;
        FloatSize frameSize;
        float frameDuration;

        template<BinaryOutput Out>
        void serialize(BinaryWriter<Out>& writer) const {
            writer.write(header);
            writer.writeU8(static_cast<uint8_t>(type));
            switch (type) {
                case Type::FrameByFrame:
                    writer.writeU64(static_cast<uint64_t>(frames.size()));
                    for (const auto& frame : frames) {
                        writer.write(frame);
                    }
                    break;
                case Type::Generated:
                    writer.write(texture);
                    writer.writeI32(rows);
                    writer.writeI32(columns);
                    writer.write(frameSize);
                    writer.writeFloat(frameDuration);
                    break;
            }
        }

        template<BinaryInput In>
        static Animation deserialize(BinaryReader<In>& reader) {
            AssetHeader header = reader.template read<AssetHeader>();
            Type type = static_cast<Type>(reader.readU8());
            switch (type) {
                case Type::FrameByFrame: {
                    uint64_t frameCount = reader.readU64();
                    std::vector<AnimationFrame> frames;
                    frames.resize(frameCount);
                    for (uint64_t i = 0; i < frameCount; i++) {
                        frames.push_back(reader.template read<AnimationFrame>());
                    }
                    return {
                        .header = std::move(header),
                        .type = type,
                        .frames = std::move(frames),
                    };
                }

                case Type::Generated: {
                    Resource texture = reader.template read<Resource>();
                    int rows = reader.readI32();
                    int columns = reader.readI32();
                    FloatSize frameSize = reader.template read<FloatSize>();
                    float frameDuration = reader.readFloat();
                    return {
                        .header = std::move(header),
                        .type = type,
                        .texture = std::move(texture),
                        .rows = rows,
                        .columns = columns,
                        .frameSize = frameSize,
                        .frameDuration = frameDuration
                    };
                }
            }

            throw BinaryReadException("animation type");
        }
    };
}

#endif //ASTERACITY_FILE_FORMATS_ANIMATION_H
