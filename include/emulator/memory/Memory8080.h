#pragma once

#include "MemoryBase.h"

#include <cstdint>
#include <vector>
#include <memory>

struct IO8080;

struct Memory8080 : public MemoryBase<Memory8080> {
    static constexpr size_t RAM_SIZE = 0x10000;

    std::vector<uint8_t> ram = std::vector<uint8_t>(RAM_SIZE);
    std::shared_ptr<IO8080> io;

    Memory8080();

    void setIO(std::shared_ptr<IO8080> ioPtr);

    uint8_t read(uint16_t address) const;
    void write(uint16_t address, uint8_t value);
    void clear();

    bool loadProgram(uint16_t startAddress, const std::vector<uint8_t>& program);
};
