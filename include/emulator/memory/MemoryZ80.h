#pragma once

#include "MemoryBase.h"

#include <cstdint>
#include <vector>
#include <iostream>

struct MemoryZ80 : public MemoryBase<MemoryZ80> {
    std::vector<uint8_t> ram = std::vector<uint8_t>(64 * 1024);
    
    uint8_t read(uint16_t address) const {
        if(address >= ram.size()) {
            return 0xFF;
        }
        return ram[address];
    }

    void write(uint16_t address, uint8_t value) {
        if(address >= ram.size()) {
            return;
        }
        ram[address] = value;
    }

    void clear() {
        std::fill(ram.begin(), ram.end(), 0);
    }

    bool loadProgram(uint16_t startAddress, const std::vector<uint8_t>& program) {
        if(startAddress + program.size() > ram.size()) {
            std::cerr << "Program too large to fit in memory" << std::endl;
            return false;
        }
        std::copy(program.begin(), program.end(), ram.begin() + startAddress);
        return true;
    }
};
