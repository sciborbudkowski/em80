#pragma once

#include "MemoryBase.h"

#include <cstdint>
#include <vector>
#include <stdexcept>

struct IO8080;

class Memory8080 : public MemoryBase {
    public:
        Memory8080() : MemoryBase(Memory8080::CPU8080_RAM_SIZE) {}

    uint8_t read(uint16_t address) const {
        if(address >= 0xF000) {
            throw std::runtime_error("Memory8080::read(uint16_t): cannot read from I/O port without access to IO8080.");
        }
        return ram[address];
    };

    uint8_t read(uint16_t address, IOInterface& io) const {
        if(address >= 0xF000) {
            uint8_t port = address & 0xFF;
            return io.in(port);
        }
        return ram[address];
    }

    void write(uint16_t address, uint8_t value) {
        if(address >= 0xF000) {
            throw std::runtime_error("Memory8080::write(uint16_t, uint8_t): cannot write to I/O port without access to IO8080.");
        }
        ram[address] = value;
    }

    void write(uint16_t address, uint8_t value, IOInterface& io) {
        if(address >= 0xF000) {
            uint8_t port = address & 0xFF;
            io.out(port, value);
        } else {
            ram[address] = value;
        }
    }

    void clear() { std::fill(ram.begin(), ram.end(), 0); }

    bool loadProgram(uint16_t startAddress, const std::vector<uint8_t>& program) {
        if(startAddress + program.size() > ram.size()) {
            throw std::runtime_error("Memory8080::loadProgram(uint16_t, const std::vector<uint8_t>&): program too large to fit in memory.");
            return false;
        }
        std::copy(program.begin(), program.end(), ram.begin() + startAddress);
        return true;
    }

    private:
        static constexpr size_t CPU8080_RAM_SIZE = 0x10000;

        std::vector<uint8_t> ram = std::vector<uint8_t>(Memory8080::CPU8080_RAM_SIZE);
};
