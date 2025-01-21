#pragma once

#include "RegistersBase.h"

#include <cstdint>
#include <iostream>

enum class RegisterPairZ80 {
    BC = 0,
    DE = 1,
    HL = 2,
    SP = 3,
    IX = 4,
    IY = 5
};

class RegistersZ80 : public RegistersBase {
    public:
        Register8 A, F, B, C, D, E, H, L;
        Register8 A_, F_, B_, C_, D_, E_, H_, L_;
        Register16 IX, IY, PC, SP, WZ;

        RegistersZ80() { reset(); }

        void reset() override {
            A = F = B = C = D = E = H = L = 0;
            A_ = F_ = B_ = C_ = D_ = E_ = H_ = L_ = 0;
            IX = IY = PC = SP = WZ = 0;
        }

        uint16_t getProgramCounter() const override { return PC; }
        void setProgramCounter(uint16_t value) override { PC = value; }

        uint16_t getStackPointer() const override { return SP; }
        void setStackPointer(uint16_t value) override { SP = value; }

        uint16_t getRegisterPair(int pair) const override {
            switch(static_cast<RegisterPairZ80>(pair)) {
                case RegisterPairZ80::BC: return (B << 8) | C;
                case RegisterPairZ80::DE: return (D << 8) | E;
                case RegisterPairZ80::HL: return (H << 8) | L;
                case RegisterPairZ80::SP: return SP;
                case RegisterPairZ80::IX: return IX;
                case RegisterPairZ80::IY: return IY;
                default: return 0;
            }
        }

        void setRegisterPair(int pair, uint16_t value) override {
            switch(static_cast<RegisterPairZ80>(pair)) {
                case RegisterPairZ80::BC: B = (value >> 8) & 0xFF; C = value & 0xFF; break;
                case RegisterPairZ80::DE: D = (value >> 8) & 0xFF; E = value & 0xFF; break;
                case RegisterPairZ80::HL: H = (value >> 8) & 0xFF; L = value & 0xFF; break;
                case RegisterPairZ80::SP: SP = value; break;
                case RegisterPairZ80::IX: IX = value; break;
                case RegisterPairZ80::IY: IY = value; break;
            }
        }

        std::map<std::string, RegisterValue> getRegisters() const override {
            return {
                {"A", RegisterValue{A.value}}, {"F", RegisterValue{F.value}}, {"B", RegisterValue{B.value}},
                {"C", RegisterValue{C.value}}, {"D", RegisterValue{D.value}}, {"E", RegisterValue{E.value}},
                {"H", RegisterValue{H.value}}, {"L", RegisterValue{L.value}}, {"IX", RegisterValue{IX.value}},
                {"IY", RegisterValue{IY.value}}, {"PC", RegisterValue{PC.value}}, {"SP", RegisterValue{SP.value}}
            };
        }

        void dumpRegisters() const override {
            std::cout << "A: " << A << "\n"
                      << "F: " << F << "\n"
                      << "B: " << B << "\n"
                      << "C: " << C << "\n"
                      << "D: " << D << "\n"
                      << "E: " << E << "\n"
                      << "H: " << H << "\n"
                      << "L: " << L << "\n"
                      << "----------\n"
                      << "IX: " << IX << "\n"
                      << "IY: " << IY << "\n"
                      << "PC: " << PC << "\n"
                      << "SP: " << SP << "\n"
                      << "WZ: " << WZ << "\n";
        }
};