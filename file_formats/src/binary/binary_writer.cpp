// Copyright 2026 Jannik Laugmand Bülow

#include "file_formats/binary/binary_writer.h"

#include <bit>
#include <cstring>

namespace formats {
    MemoryOutput::MemoryOutput(std::vector<std::byte>& out)
        : mBuffer(out) {
        mBuffer.reserve(out.size() + 256);
    }

    void MemoryOutput::write(std::byte b) {
        mBuffer.push_back(b);
    }

    void MemoryOutput::writeMany(const std::byte* bytes, size_t count) {
        mBuffer.insert(mBuffer.end(), bytes, bytes + count);
    }

    FileOutput::FileOutput(FILE* file)
        : mFile(file) {}

    void FileOutput::write(std::byte b) {
        if (fwrite(&b, sizeof(b), 1, mFile) != sizeof(b)) throw BinaryWriteException("fwrite");
    }

    void FileOutput::writeMany(const std::byte* bytes, size_t count) {
        if (fwrite(bytes, sizeof(*bytes), count, mFile) != count * sizeof(*bytes)) throw BinaryWriteException("fwrite");
    }
}
