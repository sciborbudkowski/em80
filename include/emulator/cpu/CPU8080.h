#pragma once

#include "CPUBase.h"
#include "Registers8080.h"
#include "Memory8080.h"
#include "IO8080.h"

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
            void testOpcodes();

        private:
            std::deque<std::string> lastInstructions;

            bool turboMode = false;
            const double clockSpeed = 20000000.0;
            uint64_t lastCycleTime = 0;

            std::map<uint8_t, int> instructionCycles = {
                {0x00, 4}, {0x01, 10}, {0x02, 7}, {0x03, 6}, {0x04, 4}, {0x05, 4},
                {0x06, 7}, {0x07, 4}, {0x08, 4}, {0x09, 11}, {0x0A, 7}, {0x0B, 6},
                {0x0C, 4}, {0x0D, 4}, {0x0E, 7}, {0x0F, 4}, {0x10, 4}, {0x11, 10},
                {0x12, 7}, {0x13, 6}, {0x14, 4}, {0x15, 4}, {0x16, 7}, {0x17, 4},
                {0x18, 4}, {0x19, 11}, {0x1A, 7}, {0x1B, 6}, {0x1C, 4}, {0x1D, 4},
                {0x1E, 7}, {0x1F, 4}, {0x20, 4}, {0x21, 10}, {0x22, 16}, {0x23, 6},
                {0x24, 4}, {0x25, 4}, {0x26, 7}, {0x27, 4}, {0x28, 4}, {0x29, 11},
                {0x2A, 16}, {0x2B, 6}, {0x2C, 4}, {0x2D, 4}, {0x2E, 7}, {0x2F, 4},
                {0x30, 4}, {0x31, 10}, {0x32, 13}, {0x33, 6}, {0x34, 11}, {0x35, 11},
                {0x36, 10}, {0x37, 4}, {0x38, 4}, {0x39, 11}, {0x3A, 13}, {0x3B, 6},
                {0x3C, 4}, {0x3D, 4}, {0x3E, 7}, {0x3F, 4}, {0x76, 7}
            };
    };

} // namespace CPU_8080
