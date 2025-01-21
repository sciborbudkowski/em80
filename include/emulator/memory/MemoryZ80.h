#pragma once

#include "MemoryBase.h"
#include <stdexcept>

class MemoryZ80 : public MemoryBase {
    public:
        static constexpr size_t RAM_SIZE = 0x10000;
        static constexpr size_t ROM_SIZE = 0x4000;

        MemoryZ80() : ram(RAM_SIZE, 0) {}

        uint8_t read(uint16_t address) const override {
            if(address < ROM_SIZE) {
                return rom[address];
            } else {
                return ram[address - ROM_SIZE];
            }
        }

        void write(uint16_t address, uint8_t value) override {
            if(address >= ROM_SIZE) {
                ram[address - ROM_SIZE] = value;
            } else {
                throw std::runtime_error("Attempt to write to ROM memory");
            }
        }

        void clear() override {
            std::fill(ram.begin(), ram.end(), 0);
        }

        void loadRom(const std::vector<uint8_t>& data) {
            if(data.size() > ROM_SIZE) {
                throw std::runtime_error("ROM data too large");
            }
            std::copy(data.begin(), data.end(), rom.begin());
        }

        void loadRam(const std::vector<uint8_t>& data, uint16_t startAddress = ROM_SIZE) {
            size_t offset = startAddress - ROM_SIZE;
            if(offset + data.size() > RAM_SIZE) {
                throw std::runtime_error("RAM data too large");
            }
            std::copy(data.begin(), data.end(), ram.begin() + offset);
        }

    private:
        std::vector<uint8_t> ram;
        std::vector<uint8_t> rom;
};
