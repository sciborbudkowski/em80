#pragma once

#include <cstdint>

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
