#pragma once

#include <cstdint>
#include <vector>

class MemoryBase {
    public:
        virtual ~MemoryBase() = default;

        virtual uint8_t read(uint16_t address) const = 0;
        virtual void write(uint16_t address, uint8_t value) = 0;

        virtual void clear() = 0;
};
