#pragma once

#include <cstdint>

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
