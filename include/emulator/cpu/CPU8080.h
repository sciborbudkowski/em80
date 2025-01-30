#pragma once

#include "CPUBase.h"
#include "Terminal8080.h"
#include "DiskController.h"

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
    static constexpr size_t RAM_SIZE = 0x10000;

    std::vector<uint8_t> ram = std::vector<uint8_t>(RAM_SIZE);
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
    Terminal8080& terminal;
    DiskController& disk;
    bool diskEnabled = false;
    Memory8080& memory;

    IO8080(Terminal8080& terminal, Memory8080& memory, DiskController& disk)
        : terminal(terminal), memory(memory), diskEnabled(false), disk(disk) {}

    uint8_t in(uint16_t port) const {
        if(port >= ports.size()) return 0xFF;

        switch(port) {
            case 0x02: return terminal.getLastChar(); break;
            case 0x10: return diskEnabled ? 0x00 : 0xFF; break;
            case 0x13: return diskEnabled ? disk.read() : 0xFF; break;
        }

        return ports[port];
    }

    void out(uint16_t port, uint8_t value) {
        if(port >= ports.size()) return;

        switch(port) {
            case 0x01: terminal.printChar(value); break;
            case 0x11: /* TODO: drive selection*/ break;
            case 0x12: disk.setSector(value); break;
        }

        ports[port] = value;
    }
};

class CPU8080 : public CPUBase<CPU8080> {
    public:
        std::vector<std::string> lastInstructions;

        void reset();
        void step();
        void decodeAndExecute(uint8_t opcode);
        void run();

        void pushStack(uint16_t value);
        uint16_t popStack();

        const Registers8080& getRegisters() const { return regs; }
        const Memory8080& getMemory() const { return memory; }
        const Terminal8080& getTerminal() const { return terminal; }

        IO8080& getIO() { return io; }

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
        Terminal8080& terminal;

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
