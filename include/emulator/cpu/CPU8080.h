#pragma once

#include "CPUBase.h"
#include "Registers8080.h"
#include "CPMBIOS8080.h"
#include "CPUUtils.h"
#include "IO8080.h"
#include "Memory8080.h"

#include <vector>
#include <memory>
#include <stdexcept>

class CPU8080 : public CPUBase {
    public:
        CPU8080() = default;
        ~CPU8080() = default;

        void reset();
        void step();
        void decodeAndExecute(uint8_t opcode);
        void decodeAndExecute(uint16_t opcode, uint8_t operand) { throw std::runtime_error("Not implemented: this feature is not supported by Intel 8080."); }
        void run();

        void pushStack(uint16_t value);
        uint16_t popStack();

        Registers8080& getRegisters() { return regs; }
        const Registers8080& getRegisters() const { return regs; }

        const Memory8080& getMemory() const { return memory; }

        IO8080& getIO() { return io; }
        const IO8080& getIO() const { return io; }
        
        CPMBIOS8080& getCPMBIOS() { return *cpmbios; }
        const CPMBIOS8080& getCPMBIOS() const { return *cpmbios; }

        bool isRunning() const { return running; }
        void setRunning(bool value) { running = value; }
        bool isTurboMode() const { return turboMode; }
        void setTurboMode(bool value) { turboMode = value; }

        bool loadProgram(uint16_t startAddress, std::vector<uint8_t>& program);
        void consoleDump();
        void testOpcodes();

    private:
        Registers8080 regs;
        Memory8080 memory;
        IO8080 io;

        bool running = false;
        bool turboMode = false;
        const double clockSpeed = 2000000.0;
        uint64_t lastCycleTime = 0;
        bool interruptsEnabled = false;

        const std::vector<int> instructionCycles = {
            4, 10,  7,  6,  4,  4,  7,  4,  4, 11,  7,  6,  4,  4,  7,  4,
            4, 10,  7,  6,  4,  4,  7,  4,  4, 11,  7,  6,  4,  4,  7,  4,
            4, 10,  7,  6,  4,  4,  7,  4,  4, 11,  7,  6,  4,  4,  7,  4,
            4, 10, 16,  6,  4,  4,  7,  4,  4, 11, 16,  6,  4,  4,  7,  4,
            4, 10, 16,  6,  4,  4,  7,  4,  4, 11, 16,  6,  4,  4,  7,  4,
            4, 10, 13,  6, 11, 11, 10,  4,  4, 11, 13,  6,  4,  4,  7,  4,
            4, 10, 13,  6, 11, 11, 10,  4,  4, 11, 13,  6,  4,  4,  7,  4,
            5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
            5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
            5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
            5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  7,
            4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
            4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
            4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
            4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
            4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
            4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
            4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
            4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  7
        };

        const std::vector<int> instructionTimes = {
            2,  5,  4,  3,  2,  2,  4,  2,  2,  6,  4,  3,  2,  2,  4,  2,
            2,  5,  4,  3,  2,  2,  4,  2,  2,  6,  4,  3,  2,  2,  4,  2,
            2,  5,  4,  3,  2,  2,  4,  2,  2,  6,  4,  3,  2,  2,  4,  2,
            2,  5,  4,  3,  2,  2,  4,  2,  2,  6,  4,  3,  2,  2,  4,  2,
            2,  5,  4,  3,  2,  2,  4,  2,  2,  6,  4,  3,  2,  2,  4,  2,
            2,  5,  4,  3,  2,  2,  4,  2,  2,  6,  4,  3,  2,  2,  4,  2,
            2,  5,  8,  3,  2,  2,  4,  2,  2,  6,  8,  3,  2,  2,  4,  2,
            2,  5,  8,  3,  2,  2,  4,  2,  2,  6,  8,  3,  2,  2,  4,  2,
            2,  5,  7,  3,  6,  6,  5,  2,  2,  6,  7,  3,  2,  2,  4,  2,
            2,  5,  7,  3,  6,  6,  5,  2,  2,  6,  7,  3,  2,  2,  4,  2,
            3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
            3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
            3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
            3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  4,
            2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
            2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
            2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
            2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
            2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  4
        };
};
