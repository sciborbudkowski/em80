#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>

class MemoryInterface {
    public:
        virtual ~MemoryInterface() = default;

        virtual uint8_t read(uint16_t address) const = 0;
        virtual uint8_t read(uint16_t address, IOInterface& io) const = 0;

        virtual void write(uint16_t address, uint8_t value) = 0;
        virtual void write(uint16_t address, uint8_t value, IOInterface& io) = 0;

        virtual void clear() = 0;
        virtual bool loadProgram(uint16_t startAddress, const std::vector<uint8_t>& program) = 0;
};

class MemoryBase : public MemoryInterface {
    public:
        MemoryBase(size_t size) : RAM_SIZE(size) {}

        virtual uint8_t read(uint16_t address) const = 0;
        virtual uint8_t read(uint16_t address, IOInterface& io) const = 0;

        virtual void write(uint16_t address, uint8_t value) = 0;
        virtual void write(uint16_t address, uint8_t value, IOInterface& io) = 0;
        
        virtual void clear() = 0;
        virtual bool loadProgram(uint16_t startAddress, const std::vector<uint8_t>& program) = 0;

    protected:
        size_t RAM_SIZE;
};
