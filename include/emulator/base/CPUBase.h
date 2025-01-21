#pragma once

#include "RegistersBase.h"
#include "MemoryBase.h"
#include "IOBase.h"

#include <deque>
#include <string>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <memory>

class CPUBase {
    public:
        virtual ~CPUBase() = default;

        virtual void reset() = 0;
        virtual void step() = 0;
        virtual void execute(uint16_t opcode) = 0;
        virtual bool isHalted() const = 0;
        virtual void run() = 0;

        virtual std::string hex8(uint8_t value) const {
            std::ostringstream oss;
            oss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(value);
            return oss.str();
        }

        virtual std::string hex16(uint16_t value) const {
            std::ostringstream oss;
            oss << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << value;
            return oss.str();
        }

        IOBase& getIO() { return *io; }
        MemoryBase& getMemory() { return *memory; }

    protected:
        std::unique_ptr<RegistersBase> regs;
        std::unique_ptr<MemoryBase> memory;
        std::unique_ptr<IOBase> io;
};

