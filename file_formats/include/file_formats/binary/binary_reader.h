// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_FILE_FORMATS_BINARY_BINARY_READER_H
#define ASTERACITY_FILE_FORMATS_BINARY_BINARY_READER_H

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>

namespace formats {
    template<class T>
    concept BinaryInput = requires(T& in, std::byte* bytes, size_t count) {
        { in.read() } -> std::same_as<std::byte>;
        { in.readMany(bytes, count) } -> std::same_as<void>;
    };

    template <BinaryInput In>
    class BinaryReader;

    template<class T, class Reader>
    concept Deserializable = requires(Reader& reader) {
        { T::deserialize(reader) } -> std::same_as<T>;
    };

    class BinaryReadException : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class MemoryInput {
    public:
        MemoryInput(const std::byte* bytes, size_t count);

        std::byte read();
        void readMany(std::byte* bytes, size_t count);

    private:
        std::unique_ptr<std::byte[]> mBuffer;
        size_t mBufferSize;
        size_t mCursor = 0;
    };

    class FileInput {
    public:
        explicit FileInput(FILE* file);

        std::byte read();
        void readMany(std::byte* bytes, size_t count);

    private:
        FILE* mFile;
    };

    template<BinaryInput In>
    class BinaryReader {
    public:
        template<class... Args>
        explicit BinaryReader(Args&&... args)
            : mIn(std::forward<Args>(args)...) {}

        uint8_t readU8() {
            return readLE<uint8_t>();
        }

        uint16_t readU16() {
            return readLE<uint16_t>();
        }

        uint32_t readU32() {
            return readLE<uint32_t>();
        }

        uint64_t readU64() {
            return readLE<uint64_t>();
        }

        int8_t readI8() {
            return readLE<int8_t>();
        }

        int16_t readI16() {
            return readLE<int16_t>();
        }

        int32_t readI32() {
            return readLE<int32_t>();
        }

        int64_t readI64() {
            return readLE<int64_t>();
        }

        float readFloat() {
            static_assert(sizeof(float) == 4);
            static_assert(std::numeric_limits<float>::is_iec559);
            uint32_t bits = readU32();
            return std::bit_cast<float>(bits);
        }

        double readDouble() {
            static_assert(sizeof(double) == 8);
            static_assert(std::numeric_limits<double>::is_iec559);
            uint64_t bits = readU64();
            return std::bit_cast<double>(bits);
        }

        std::pair<int, int> readVec2i() {
            int x = readI32();
            int y = readI32();
            return {x, y};
        }

        std::pair<float, float> readVec2f() {
            float x = readFloat();
            float y = readFloat();
            return {x, y};
        }

        std::string readString() {
            uint16_t size = readU16();
            std::string value;
            value.resize(size);
            mIn.readMany(value.data(), size);
            return value;
        }

        template<Deserializable<BinaryReader> T>
        T read() {
            return T::deserialize(*this);
        }

    private:
        In mIn;

        template<class T>
        requires (std::is_integral_v<T>)
        T readLE() {
            std::array<std::byte, sizeof(T)> bytes;
            mIn.readMany(bytes.data(), bytes.size());

            using U = std::make_unsigned_t<T>;
            U value = 0;

            for (std::size_t i = 0; i < bytes.size(); ++i) {
                value |= static_cast<U>(std::to_integer<unsigned char>(bytes[i])) << (i * 8);
            }

            return static_cast<T>(value);

        }
    };
}

#endif //ASTERACITY_FILE_FORMATS_BINARY_BINARY_READER_H
