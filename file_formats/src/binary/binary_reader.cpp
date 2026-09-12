// Copyright 2026 Jannik Laugmand Bülow

#include "file_formats/binary/binary_reader.h"

namespace formats {
    MemoryInput::MemoryInput(const std::byte* bytes, size_t count)
        : mBuffer(std::make_unique<std::byte[]>(count))
        , mBufferSize(count) {
        memcpy(mBuffer.get(), bytes, mBufferSize);
    }

    std::byte MemoryInput::read() {
        if (mCursor >= mBufferSize) throw BinaryReadException("overflow");
        return mBuffer[mCursor++];
    }

    void MemoryInput::readMany(std::byte* bytes, size_t count) {
        if (mCursor + count >= mBufferSize) throw BinaryReadException("overflow");
        memcpy(bytes, mBuffer.get() + mCursor, count);
        mCursor += count;
    }


    FileInput::FileInput(FILE* file)
        : mFile(file) {}

    std::byte FileInput::read() {
        std::byte result;
        if (fread(&result, sizeof(result), 1, mFile) != sizeof(result)) throw BinaryReadException("fread");
        return result;
    }

    void FileInput::readMany(std::byte* bytes, size_t count) {
        if (fread(bytes, sizeof(*bytes), count, mFile) != sizeof(*bytes) * count) throw BinaryReadException("fread");
    }
}
