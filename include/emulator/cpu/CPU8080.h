#pragma once

#include "CPUBase.h"
#include "Registers8080.h"
#include "CPUUtils.h"
#include "CPMBIOS.h"

#include <vector>
#include <memory>
#include <stdexcept>

class Terminal8080;
class Memory8080;
class IO8080;

class CPU8080 : public CPUBase<CPU8080> {
    public:
        CPU8080(
            std::shared_ptr<Terminal8080> terminalPtr,
            std::shared_ptr<Memory8080> memoryPtr,
            std::shared_ptr<IO8080> ioPtr
        ) : terminal(terminalPtr), memory(memoryPtr), io(ioPtr) {
            utils = CPUUtils();
        }

        ~CPU8080() = default;

        std::vector<std::string> lastInstructions;

        void reset();
        void step();
        void decodeAndExecute(uint8_t opcode);
        void decodeAndExecute(uint16_t opcode, uint8_t operand) { throw std::runtime_error("Not implemented: this feature is not supported by Intel 8080."); }
        void run();

        void pushStack(uint16_t value);
        uint16_t popStack();

        const Registers8080& getRegisters() const { return regs; }
        Registers8080& getRegisters() { return regs; }
        const Memory8080& getMemory() const { return *memory; }
        const Terminal8080& getTerminal() const { return *terminal; }
        IO8080& getIO() { return *io; }

        bool isRunning() const { return running; }
        void setRunning(bool value) { running = value; }
        bool isTurboMode() const { return turboMode; }
        void setTurboMode(bool value) { turboMode = value; }

        bool loadProgram(uint16_t startAddress, std::vector<uint8_t>& program);
        void consoleDump();
        void testOpcodes();

    private:
        Registers8080 regs;
        std::shared_ptr<Memory8080> memory;
        std::shared_ptr<IO8080> io;
        std::shared_ptr<Terminal8080> terminal;
        std::shared_ptr<CPMBIOS> cpmbios;
        CPUUtils utils;

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
