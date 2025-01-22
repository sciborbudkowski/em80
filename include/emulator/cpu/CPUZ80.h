#pragma once

#include "CPUBase.h"
#include "RegistersZ80.h"
#include "MemoryZ80.h"
#include "IOZ80.h"

namespace CPU_Z80 {

    class CPUZ80 : public CPUBase {
        public:
            RegistersZ80 regs;
            MemoryZ80 memory;
            IOZ80 io;
            bool interruptEnabled;

            CPUZ80() = default;
            CPUZ80(const CPUZ80&) = delete;
            CPUZ80& operator=(const CPUZ80&) = delete;
            CPUZ80(CPUZ80&&) = delete;
            CPUZ80& operator=(CPUZ80&&) = delete;

            void reset() override;
            void step() override;
            void execute(uint8_t opcode) override;
            void execute(uint8_t opcode, uint8_t subOpcode) override;
            void run() override;

            void pushStack(uint16_t value);
            uint16_t popStack();

            bool loadProgram(const std::string& filename, uint16_t startAddress);

            void consoleDump();
            void testOpcodes();

        private:
            bool halted = false;
            std::deque<std::string> lastInstructions;

            const double clockSpeed = 40000000.0;
            uint64_t lastCycleTime = 0;

            std::map<uint8_t, int> cycles {
                {0x00, 1}
            };
    };

} // namespace CPU_Z80
