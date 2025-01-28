#pragma once

#include "CPUBase.h"
#include "Registers8080.h"
#include "Memory8080.h"
#include "IO8080.h"

#include <vector>

namespace CPU_8080 {

    class CPU8080 : public CPUBase {
        public:
            Registers8080 regs;
            Memory8080 memory;
            IO8080 io;
            bool interruptEnabled;

            CPU8080() = default;
            CPU8080(const CPU8080&) = delete;
            CPU8080& operator=(const CPU8080&) = delete;
            CPU8080(CPU8080&&) = delete;
            CPU8080& operator=(CPU8080&&) = delete;

            void reset() override;
            void step() override;
            void execute(uint8_t opcode) override;
            void execute(uint8_t opcode, uint8_t subOpcode) override;
            void run() override;
            void switchTurbo();
            void pushStack(uint16_t value) override;
            uint16_t popStack() override;

            bool loadProgram(const std::string& filename, uint16_t startAddress);

            void consoleDump();
            void testOpcodes() override;

        private:
            std::deque<std::string> lastInstructions;

            bool turboMode = false;
            const double clockSpeed = 20000000.0;
            uint64_t lastCycleTime = 0;

            std::vector<int> instructionCycles = {
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
    };

} // namespace CPU_8080
