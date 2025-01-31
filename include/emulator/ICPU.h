#pragma once

#include <string>
#include <cstdint>

class ICPU {
    public:
        virtual void reset() = 0;
        virtual void step() = 0;
        virtual void decodeAndExecute(uint16_t opcode) = 0;
        virtual void decodeAndExecute(uint16_t opcode, uint8_t operand) = 0;
        virtual void run() = 0;

        virtual void addLastInstruction(const std::string& instruction) = 0;
        virtual void dumpLastInstructions(int num = 8) const = 0;
};
