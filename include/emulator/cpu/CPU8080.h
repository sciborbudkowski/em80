#pragma once

#include "CPUBase.h"

#include <vector>

struct Registers8080 {
    uint8_t A, B, C, D, E, H, L;
    uint16_t SP, PC;
    uint8_t Flags;

    enum Flag {
        CARRY     = (1 << 0),
        PARITY    = (1 << 2),
        AUX_CARRY = (1 << 4),
        ZERO      = (1 << 6),
        SIGN      = (1 << 7)
    };

    bool getFlag(Flag flag) const { return Flags & flag; }

    void setFlag(Flag flag, bool value) {
        if(value) { Flags |= flag; }
        else { Flags &= ~flag; }
    }

    void reset() {
        A = B = C = D = E = H = L = 0;
        SP = PC = 0;
        Flags = 0;
    }

    uint16_t HL() const { return (H << 8) | L; }
    uint16_t BC() const { return (B << 8) | C; }
    uint16_t DE() const { return (D << 8) | E; }

    void HL(uint16_t value) {
        H = (value >> 8) & 0xFF;
        L = value & 0xFF;
    }

    void BC(uint16_t value) {
        B = (value >> 8) & 0xFF;
        C = value & 0xFF;
    }

    void DE(uint16_t value) {
        D = (value >> 8) & 0xFF;
        E = value & 0xFF;
    }
};

struct Memory8080 {
    std::vector<uint8_t> ram = std::vector<uint8_t>(64 * 1024);
    IO8080& io;

    Memory8080(IO8080& io) : io(io) {}

    uint8_t read(uint16_t address) const {
        if(address >= 0xF000) {
            uint8_t port = address & 0xFF;
            return io.in(port);
        }
        return ram[address];
    }

    void write(uint16_t address, uint8_t value) {
        if(address >= 0xF000) {
            uint8_t port = address & 0xFF;
            io.out(port, value);
        } else {
            ram[address] = value;
        }
    }

    void clear() {
        std::fill(ram.begin(), ram.end(), 0);
    }

    bool loadProgram(uint16_t startAddress, const std::vector<uint8_t>& program) {
        if(startAddress + program.size() > ram.size()) {
            std::cerr << "Program too large to fit in memory" << std::endl;
            return false;
        }
        std::copy(program.begin(), program.end(), ram.begin() + startAddress);
        return true;
    }
};

struct IO8080 {
    std::vector<uint8_t> ports = std::vector<uint8_t>(256);

    uint8_t in(uint16_t port) const {
        if(port >= ports.size()) return 0xFF;
        return ports[port];
    }

    void out(uint16_t port, uint8_t value) {
        if(port >= ports.size()) return;
        ports[port] = value;
    }
};

class CPU8080 : public CPUBase<CPU8080> {
    public:
        void reset();
        void step();
        void decodeAndExecute(uint8_t opcode);
        void run();

        void pushStack(uint16_t value);
        uint16_t popStack();

        const Registers8080& getRegisters() const { return regs; }
        const Memory8080& getMemory() { return memory; }
        const IO8080& getIO() { return io; }

        bool isRunning() const { return running; }
        void setRunning(bool value) { running = value; }
        bool isTurboMode() const { return turboMode; }
        void setTurboMode(bool value) { turboMode = value; }

        bool loadProgram(uint16_t startAddress, std::vector<uint8_t>& program);
        void consoleDump();
        void testOpcodes();

    private:
        Registers8080 regs;
        Memory8080& memory;
        IO8080& io;

        bool running = false;
        bool turboMode = false;
        const double clockSpeed = 20000000.0;
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
};

/*
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
*/