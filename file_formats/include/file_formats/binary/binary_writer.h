// Copyright 2026 Jannik Laugmand Bülow

#ifndef ASTERACITY_FILE_FORMATS_BINARY_BINARY_WRITER_H
#define ASTERACITY_FILE_FORMATS_BINARY_BINARY_WRITER_H

#include <cstdint>
#include <cstdio>
#include <span>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace formats {
    template<class T>
    concept BinaryOutput = requires(T& out, std::byte b, const std::byte* bytes, size_t count) {
        { out.write(b) } -> std::same_as<void>;
        { out.writeMany(bytes, count) } -> std::same_as<void>;
    };

    template<BinaryOutput Out>
    class BinaryWriter;

    template<class T, class Writer>
    concept Serializable = requires(const T& value, Writer& writer) {
        { value.serialize(writer) } -> std::same_as<void>;
    };

    class BinaryWriteException : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class MemoryOutput {
    public:
        explicit MemoryOutput(std::vector<std::byte>& out);

        void write(std::byte b);
        void writeMany(const std::byte* bytes, size_t count);

    private:
        std::vector<std::byte>& mBuffer;
    };

    class FileOutput {
    public:
        explicit FileOutput(FILE* file);

        void write(std::byte b);
        void writeMany(const std::byte* bytes, size_t count);

    private:
        FILE* mFile;
    };

    template<BinaryOutput Out>
    class BinaryWriter {
    public:
        template<class... Args>
        explicit BinaryWriter(Args&&... args)
            : mOut(std::forward<Args>(args)...) {};

        void writeU8(uint8_t value) {
            writeLE(value);
        }

        void writeU16(uint16_t value) {
            writeLE(value);
        }

        void writeU32(uint32_t value) {
            writeLE(value);
        }

        void writeU64(uint64_t value) {
            writeLE(value);
        }

        void writeI8(int8_t value) {
            writeLE(value);
        }

        void writeI16(int16_t value) {
            writeLE(value);
        }

        void writeI32(int32_t value) {
            writeLE(value);
        }

        void writeI64(int64_t value) {
            writeLE(value);
        }

        void writeFloat(float value) {
            static_assert(sizeof(float) == 4);
            static_assert(std::numeric_limits<float>::is_iec559);
            writeU32(std::bit_cast<uint32_t>(value));
        }

        void writeDouble(double value) {
            static_assert(sizeof(float) == 4);
            static_assert(std::numeric_limits<float>::is_iec559);
            writeU64(std::bit_cast<uint64_t>(value));
        }

        void writeVec2i(int x, int y) {
            writeI32(x);
            writeI32(y);
        }

        void writeVec2f(float x, float y) {
            writeFloat(x);
            writeFloat(y);
        }

        void writeString(std::string_view value) {
            // hope and pray that the string is no longer than 0xffff
            writeU16(static_cast<uint16_t>(value.length()));
            mOut.writeMany(reinterpret_cast<const std::byte*>(value.data()), value.length());
        }

        template<Serializable<BinaryWriter> T>
        void write(T value) {
            value.serialize(*this);
        }

    private:
        Out mOut;

        template<class T>
        requires (std::is_integral_v<T>)
        void writeLE(T value) {
            using U = std::make_unsigned_t<T>;
            U u = static_cast<U>(value);

            std::byte bytes[sizeof(T)];

            for (std::size_t i = 0; i < sizeof(T); i++) {
                bytes[i] = static_cast<std::byte>(static_cast<unsigned char>(u >> (i * 8)));
            }

            mOut.writeMany(bytes, sizeof(bytes));
        }
    };
}

#endif //ASTERACITY_FILE_FORMATS_BINARY_BINARY_WRITER_H
