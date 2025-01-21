#pragma once

#include "CPUBase.h"
#include "RegistersZ80.h"
#include "MemoryZ80.h"
#include "IOZ80.h"

#define CPU_NAME "Z80"

class CPUZ80 : public CPUBase {
    public:
        RegistersZ80 regs;
        MemoryZ80 memory;
        IOZ80 io;

        CPUZ80();
        CPUZ80(const CPUZ80&) = delete;
        CPUZ80& operator=(const CPUZ80&) = delete;
        CPUZ80(CPUZ80&&) = delete;
        CPUZ80& operator=(CPUZ80&&) = delete;

        void reset();
        void step();
        void execute(uint16_t opcode);
        bool isHalted() const;
        void run();

        bool loadProgram(const std::string& filename, uint16_t startAddress);

        void consoleDump();
        void testOpcodes();

        private:
            bool halted = false;
            std::deque<std::string> lastInstructions;

            void push(uint16_t value);
            uint16_t pop();

            const double clockSpeed = 40000000.0;
            uint64_t lastCycleTime = 0;
};