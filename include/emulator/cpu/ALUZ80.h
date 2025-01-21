#include "RegistersZ80.h"

class ALUZ80 {
    public:
        static void ex_ax_ax(RegistersZ80& regs) {
            std::swap(regs.A, regs.A_);
            std::swap(regs.F, regs.F_);
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
