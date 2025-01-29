#pragma once

#include "CPUZ80.h"

struct ALUZ80 {
    enum Flag {
        CARRY      = (1 << 0),
        SUBTRACT   = (1 << 1),
        PARITY     = (1 << 2),
        HALF_CARRY = (1 << 4),
        ZERO       = (1 << 6),
        SIGN       = (1 << 7)
    };

    static uint16_t calculateAddress(uint8_t high, uint8_t low) { return (high << 8) | low; }

    static void ex_ax_ax(RegistersZ80& regs) {
        std::swap(regs.A, regs.A_);
        std::swap(regs.Flags, regs.Flags_);
    }

    static void exx(RegistersZ80& regs) {
        std::swap(regs.B, regs.B_);
        std::swap(regs.C, regs.C_);
        std::swap(regs.D, regs.D_);
        std::swap(regs.E, regs.E_);
        std::swap(regs.H, regs.H_);
        std::swap(regs.L, regs.L_);
    }
};
