#pragma once
#include <angelscript.h>
#include <string>
#include <cstring>
#include <algorithm>

template <typename IdT, typename BufT = std::string>
struct ByteCodeMemory : public asIBinaryStream {
    BufT data;
    size_t pos = 0;
    IdT id{};
    std::string name;

    ByteCodeMemory() = default;
    ByteCodeMemory(IdT identifier, const char* moduleName)
        : data(), pos(0), id(identifier), name(moduleName ? moduleName : "") {}

    void resetReadPos() { pos = 0; }
    void resetWritePos() { pos = 0; }

    IdT getID() const { return id; }
    char* getName() { return name.empty() ? nullptr : const_cast<char*>(name.c_str()); }

    int Write(const void* ptr, asUINT size) override {
        if (pos + size > data.size()) data.resize(pos + size);
        std::memcpy(&data[pos], ptr, size);
        pos += size;
        return static_cast<int>(size);
    }

    int Read(void* ptr, asUINT size) override {
        asUINT avail = static_cast<asUINT>(std::min<size_t>(size, data.size() - pos));
        if (avail > 0) {
            std::memcpy(ptr, &data[pos], avail);
            pos += avail;
        }
        return static_cast<int>(avail);
    }
};
