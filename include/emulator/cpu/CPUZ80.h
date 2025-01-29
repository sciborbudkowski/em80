#pragma once

#include "CPUBase.h"

#include <vector>
#include <cstdint>
#include <iostream>
#include <array>

struct RegistersZ80 {
    uint8_t A, B, C, D, E, H, L;
    uint8_t A_, B_, C_, D_, E_, H_, L_;
    uint16_t IX, IY, PC, SP, WZ;
    uint8_t Flags, Flags_;

    enum Flag {
        CARRY      = (1 << 0),
        SUBTRACT   = (1 << 1),
        PARITY     = (1 << 2),
        HALF_CARRY = (1 << 4),
        ZERO       = (1 << 6),
        SIGN       = (1 << 7)
    };

    bool getFlag(Flag flag) const { return Flags & flag; }

    void setFlag(Flag flag, bool value) {
        if(value) { Flags |= flag; }
        else { Flags &= ~flag; }
    }

    RegistersZ80() { reset(); }

    void reset() {
        A = B = C = D = E = H = L = 0;
        A_ = B_ = C_ = D_ = E_ = H_ = L_ = 0;
        IX = IY = PC = SP = WZ = 0;
    }

    uint16_t AF() const { return (A << 8) | Flags; }
    uint16_t BC() const { return (B << 8) | C; }
    uint16_t DE() const { return (D << 8) | E; }
    uint16_t HL() const { return (H << 8) | L; }

    void AF(uint16_t value) { A = (value >> 8) & 0xFF; Flags = value & 0xFF; }
    void BC(uint16_t value) { B = (value >> 8) & 0xFF; C = value & 0xFF; }
    void DE(uint16_t value) { D = (value >> 8) & 0xFF; E = value & 0xFF; }
    void HL(uint16_t value) { H = (value >> 8) & 0xFF; L = value & 0xFF; }
};

struct MemoryZ80 {
    std::vector<uint8_t> ram = std::vector<uint8_t>(64 * 1024);
    
    uint8_t read(uint16_t address) const {
        if(address >= ram.size()) {
            return 0xFF;
        }
        return ram[address];
    }

    void write(uint16_t address, uint8_t value) {
        if(address >= ram.size()) {
            return;
        }
        ram[address] = value;
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

struct IOZ80 {
    std::vector<uint8_t> ports = std::vector<uint8_t>(0xFF, 0xFF);

    uint8_t in(uint8_t port) const { return ports[port]; }

    void out(uint8_t port, uint8_t value) { ports[port] = value; }
};


class CPUZ80 : public CPUBase<CPUZ80> {
    public:
        void reset();
        void step();
        void decodeAndExecute(uint8_t opcode, uint8_t subOpcode = 0x00);
        void run();

        void pushStack(uint16_t value);
        uint16_t popStack();

        const RegistersZ80& getRegisters() const { return regs; }
        const MemoryZ80& getMemory() { return memory; }
        const IOZ80& getIO() { return io; }

        bool isRunning() const { return running; }
        void setRunning(bool value) { running = value; }
        bool isTurboMode() const { return turboMode; }
        void setTurboMode(bool value) { turboMode = value; }

        bool loadProgram(uint16_t startAddress, const std::vector<uint8_t>& program);
        void consoleDump();
        void testOpcodes();

    private:
        RegistersZ80 regs;
        MemoryZ80 memory;
        IOZ80 io;

        bool running = false;
        bool turboMode = false;
        const double clockSpeed = 40000000.0;
        uint64_t lastCycleTime = 0;
        bool interruptsEnabled = false;
        enum OpcodePrefix {
            BASE = 0,
            CB   = 1,
            DD   = 2,
            ED   = 3,
            FD   = 4
        };

        std::array<std::array<uint8_t, 256>, 5> instructionCycles = {{
        {
            {
                // Główna tablica (brak prefiksu) 0x00-0xFF
                 4,10, 7, 6, 4, 4, 7, 4,   4,11, 7, 6, 4, 4, 7, 4,     // 0x00-0x0F
                 8,10, 7, 6, 4, 4, 7, 4,  12,11, 7, 6, 4, 4, 7, 4,     // 0x10-0x1F
                 8,10,16, 6, 4, 4, 7, 4,   8,11,16, 6, 4, 4, 7, 4,     // 0x20-0x2F
                 8,10,13, 6,19,19,15, 4,   8,11,13, 6, 4, 4, 7, 4,     // 0x30-0x3F
                 4, 4, 4, 4, 4, 4, 7, 4,   4, 4, 4, 4, 4, 4, 7, 4,     // 0x40-0x4F
                 4, 4, 4, 4, 4, 4, 7, 4,   4, 4, 4, 4, 4, 4, 7, 4,     // 0x50-0x5F
                 4, 4, 4, 4, 4, 4, 7, 4,   4, 4, 4, 4, 4, 4, 7, 4,     // 0x60-0x6F
                 7, 7, 7, 7, 7, 7, 7, 7,   4, 4, 4, 4, 4, 4, 7, 4,     // 0x70-0x7F
                 4, 4, 4, 4, 4, 4, 7, 4,   4, 4, 4, 4, 4, 4, 7, 4,     // 0x80-0x8F
                 4, 4, 4, 4, 4, 4, 7, 4,   4, 4, 4, 4, 4, 4, 7, 4,     // 0x90-0x9F
                 4, 4, 4, 4, 4, 4, 7, 4,   4, 4, 4, 4, 4, 4, 7, 4,     // 0xA0-0xAF
                 4, 4, 4, 4, 4, 4, 7, 4,   4, 4, 4, 4, 4, 4, 7, 4,     // 0xB0-0xBF
                11,10,10,10,17,11, 7,11,  11,10,10,10,17,17, 7,11,     // 0xC0-0xCF
                11,10,10,10,17,11, 7,11,  11,10,10,10,17,17, 7,11,     // 0xD0-0xDF
                11,10,10,18,17,11, 7,11,  11, 4,10, 4,17,17, 7,11,     // 0xE0-0xEF
                11,10,10, 4,17,11, 7,11,  11, 4,10, 4,17,17, 7,11      // 0xF0-0xFF
            }},
            {{
                // Prefiks CB (bitowe operacje i przesunięcia)
                 8, 8, 8, 8, 8, 8,15, 8,   8, 8, 8, 8, 8, 8,15, 8,      // 0x00-0x0F
                 8, 8, 8, 8, 8, 8,15, 8,   8, 8, 8, 8, 8, 8,15, 8,      // 0x10-0x1F
                 8, 8, 8, 8, 8, 8,15, 8,   8, 8, 8, 8, 8, 8,15, 8,      // 0x20-0x2F
                 8, 8, 8, 8, 8, 8,15, 8,   8, 8, 8, 8, 8, 8,15, 8,      // 0x30-0x3F
                 8, 8, 8, 8, 8, 8,12, 8,   8, 8, 8, 8, 8, 8,12, 8,      // 0x40-0x4F
                 8, 8, 8, 8, 8, 8,12, 8,   8, 8, 8, 8, 8, 8,12, 8,      // 0x50-0x5F
                 8, 8, 8, 8, 8, 8,12, 8,   8, 8, 8, 8, 8, 8,12, 8,      // 0x60-0x6F
                 8, 8, 8, 8, 8, 8,12, 8,   8, 8, 8, 8, 8, 8,12, 8,      // 0x70-0x7F
                 8, 8, 8, 8, 8, 8,15, 8,   8, 8, 8, 8, 8, 8,15, 8,      // 0x80-0x8F
                 8, 8, 8, 8, 8, 8,15, 8,   8, 8, 8, 8, 8, 8,15, 8,      // 0x90-0x9F
                 8, 8, 8, 8, 8, 8,15, 8,   8, 8, 8, 8, 8, 8,15, 8,      // 0xA0-0xAF
                 8, 8, 8, 8, 8, 8,15, 8,   8, 8, 8, 8, 8, 8,15, 8,      // 0xB0-0xBF
                 8, 8, 8, 8, 8, 8,15, 8,   8, 8, 8, 8, 8, 8,15, 8,      // 0xC0-0xCF
                 8, 8, 8, 8, 8, 8,15, 8,   8, 8, 8, 8, 8, 8,15, 8,      // 0xD0-0xDF
                 8, 8, 8, 8, 8, 8,15, 8,   8, 8, 8, 8, 8, 8,15, 8,      // 0xE0-0xEF
                 8, 8, 8, 8, 8, 8,15, 8,   8, 8, 8, 8, 8, 8,15, 8       // 0xF0-0xFF
            }},
            {{
                // Prefiks DD (IX) - większość jak główna tablica, z wyjątkami
                 0, 0, 0, 0, 0, 0, 0, 0,   0,15, 0, 0, 0, 0, 0, 0,      // 0x00-0x0F
                 0, 0, 0, 0, 0, 0, 0, 0,   0,15, 0, 0, 0, 0, 0, 0,      // 0x10-0x1F
                 0,14, 0, 0, 0, 0, 0, 0,   0,15, 0, 0, 0, 0, 0, 0,      // 0x20-0x2F
                 0,14, 0, 0, 0, 0, 0, 0,   0,15, 0, 0, 0, 0, 0, 0,      // 0x30-0x3F
                 0, 0, 0, 0, 8, 8,19, 0,   0, 0, 0, 0, 8, 8,19, 0,      // 0x40-0x4F
                 0, 0, 0, 0, 8, 8,19, 0,   0, 0, 0, 0, 8, 8,19, 0,      // 0x50-0x5F
                 0, 0, 0, 0, 8, 8,19, 0,   0, 0, 0, 0, 8, 8,19, 0,      // 0x60-0x6F
                19,19,19,19,19,19,19,19,   0, 0, 0, 0, 0, 0, 0, 0,      // 0x70-0x7F
                 0, 0, 0, 0, 8, 8,19, 0,   0, 0, 0, 0, 8, 8,19, 0,      // 0x80-0x8F
                 0, 0, 0, 0, 8, 8,19, 0,   0, 0, 0, 0, 8, 8,19, 0,      // 0x90-0x9F
                 0, 0, 0, 0, 8, 8,19, 0,   0, 0, 0, 0, 8, 8,19, 0,      // 0xA0-0xAF
                 0, 0, 0, 0, 8, 8,19, 0,   0, 0, 0, 0, 8, 8,19, 0,      // 0xB0-0xBF
                 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,      // 0xC0-0xCF
                 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,      // 0xD0-0xDF
                 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,      // 0xE0-0xEF
                 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0       // 0xF0-0xFF
            }},
            {{
                // Prefiks ED (rozszerzone instrukcje)
                 8, 8, 8, 8, 8, 8, 8, 8,   8, 8, 8, 8, 8, 8, 8, 8,      // 0x00-0x0F
                 8, 8, 8, 8, 8, 8, 8, 8,   8, 8, 8, 8, 8, 8, 8, 8,      // 0x10-0x1F
                12,12,12,12, 8, 8, 8, 8,  12,12,12,12, 8, 8, 8, 8,      // 0x20-0x2F
                12,12,12,12, 8, 8, 8, 8,  12,12,12,12, 8, 8, 8, 8,      // 0x30-0x3F
                 8, 8, 8, 8, 8, 8, 8, 8,   8, 8, 8, 8, 8, 8, 8, 8,      // 0x40-0x4F
                 8, 8, 8, 8, 8, 8, 8, 8,   8, 8, 8, 8, 8, 8, 8, 8,      // 0x50-0x5F
                16,16,16,16, 8, 8, 8, 8,  16,16,16,16, 8, 8, 8, 8,      // 0x60-0x6F
                16,16,16,16, 8, 8, 8, 8,  16,16,16,16, 8, 8, 8, 8,      // 0x70-0x7F
                 8, 8, 8, 8, 8, 8, 8, 8,   8, 8, 8, 8, 8, 8, 8, 8,      // 0x80-0x8F
                 8, 8, 8, 8, 8, 8, 8, 8,   8, 8, 8, 8, 8, 8, 8, 8,      // 0x90-0x9F
                21,21,16,16, 8, 8, 8, 8,  21,21,16,16, 8, 8, 8, 8,      // 0xA0-0xAF
                21,21,16,16, 8, 8, 8, 8,  21,21,16,16, 8, 8, 8, 8,      // 0xB0-0xBF
                 8, 8, 8, 8, 8, 8, 8, 8,   8, 8, 8, 8, 8, 8, 8, 8,      // 0xC0-0xCF
                 8, 8, 8, 8, 8, 8, 8, 8,   8, 8, 8, 8, 8, 8, 8, 8,      // 0xD0-0xDF
                 8, 8, 8, 8, 8, 8, 8, 8,   8, 8, 8, 8, 8, 8, 8, 8,      // 0xE0-0xEF
                 8, 8, 8, 8, 8, 8, 8, 8,   8, 8, 8, 8, 8, 8, 8, 8       // 0xF0-0xFF
            }},
            {{
                // Prefiks FD (IY) - analogicznie do DD
                 0, 0, 0, 0, 0, 0, 0, 0,   0,15, 0, 0, 0, 0, 0, 0,      // 0x00-0x0F
                 0, 0, 0, 0, 0, 0, 0, 0,   0,15, 0, 0, 0, 0, 0, 0,      // 0x10-0x1F
                 0, 14,0, 0, 0, 0, 0, 0,   0,15, 0, 0, 0, 0, 0, 0,      // 0x20-0x2F
                 0, 14,0, 0, 0, 0, 0, 0,   0,15, 0, 0, 0, 0, 0, 0,      // 0x30-0x3F
                 0, 0, 0, 0, 8, 8,19, 0,   0, 0, 0, 0, 8, 8,19, 0,      // 0x40-0x4F
                 0, 0, 0, 0, 8, 8,19, 0,   0, 0, 0, 0, 8, 8,19, 0,      // 0x50-0x5F
                 0, 0, 0, 0, 8, 8,19, 0,   0, 0, 0, 0, 8, 8,19, 0,      // 0x60-0x6F
                19,19,19,19,19,19,19,19,   0, 0, 0, 0, 0, 0, 0, 0,      // 0x70-0x7F
                 0, 0, 0, 0, 8, 8,19, 0,   0, 0, 0, 0, 8, 8,19, 0,      // 0x80-0x8F
                 0, 0, 0, 0, 8, 8,19, 0,   0, 0, 0, 0, 8, 8,19, 0,      // 0x90-0x9F
                 0, 0, 0, 0, 8, 8,19, 0,   0, 0, 0, 0, 8, 8,19, 0,      // 0xA0-0xAF
                 0, 0, 0, 0, 8, 8,19, 0,   0, 0, 0, 0, 8, 8,19, 0,      // 0xB0-0xBF
                 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,      // 0xC0-0xCF
                 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,      // 0xD0-0xDF
                 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,      // 0xE0-0xEF
                 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0       // 0xF0-0xFF
            }}
        }
    };
};
