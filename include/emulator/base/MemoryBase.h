#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>

template <typename Derived>
struct MemoryBase {
    size_t RAM_SIZE;

    MemoryBase() = default;
    virtual ~MemoryBase() = default;

    uint8_t read(uint16_t address) const { return static_cast<Derived*>(this)->read(address); }
    void write(uint16_t address, uint8_t value) { static_cast<Derived*>(this)->write(address, value); }

    void clear() { static_cast<Derived*>(this)->clear(); }

    bool loadProgram(uint16_t startAddress, const std::vector<uint8_t>& program) { return static_cast<Derived*>(this)->loadProgram(startAddress, program); }
};
