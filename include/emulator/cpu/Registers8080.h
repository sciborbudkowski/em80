#pragma once

#include "RegistersBase.h"

#include <cstdint>
#include <iostream>

enum class RegisterPair8080 {
    BC = 0,
    DE = 1,
    HL = 2,
    SP = 3
};

class Registers8080 : public RegistersBase {
    public:
        Register8 A, B, C, D, E, H, L;
        Flag Z, S, P, CY, AC;
        Register16 PC, SP;

        Registers8080() { reset(); }

        void reset() override {
            A = B = C = D = E = H = L = 0;
            PC = SP = 0;
            Z = S = P = CY = AC = false;
        }

        uint16_t getProgramCounter() const override { return PC; }
        void setProgramCounter(uint16_t value) override { PC = value; }

        uint16_t getStackPointer() const override { return SP; }
        void setStackPointer(uint16_t value) override { SP = value; }

        uint16_t getRegisterPair(int pair) const override {
            switch(static_cast<RegisterPair8080>(pair)) {
                case RegisterPair8080::BC: return (B << 8) | C;
                case RegisterPair8080::DE: return (D << 8) | E;
                case RegisterPair8080::HL: return (H << 8) | L;
                case RegisterPair8080::SP: return SP;
                default: return 0;
            }
        }

        void setRegisterPair(int pair, uint16_t value) override {
            switch(static_cast<RegisterPair8080>(pair)) {
                case RegisterPair8080::BC: B = (value >> 8) & 0xFF; C = value & 0xFF; break;
                case RegisterPair8080::DE: D = (value >> 8) & 0xFF; E = value & 0xFF; break;
                case RegisterPair8080::HL: H = (value >> 8) & 0xFF; L = value & 0xFF; break;
                case RegisterPair8080::SP: SP = value; break;
            }
        }

        std::map<std::string, RegisterValue> getRegisters() const override {
            return {
                {"A", RegisterValue{A.value}}, {"B", RegisterValue{B.value}}, {"C", RegisterValue{C.value}},
                {"D", RegisterValue{D.value}}, {"E", RegisterValue{E.value}}, {"H", RegisterValue{H.value}},
                {"L", RegisterValue{L.value}}, {"PC", RegisterValue{PC.value}}, {"SP", RegisterValue{SP.value}},
                {"Z", RegisterValue{Z.value}}, {"S", RegisterValue{S.value}}, {"P", RegisterValue{P.value}},
                {"CY", RegisterValue{CY.value}}, {"AC", RegisterValue{AC.value}}
            };
        }

        void dumpRegisters() const override {
            std::cout << "A: " << A << "\n"
                      << "B: " << B << "\n"
                      << "C: " << C << "\n"
                      << "D: " << D << "\n"
                      << "E: " << E << "\n"
                      << "H: " << H << "\n"
                      << "L: " << L << "\n"
                      << "----------\n"
                      << "PC: " << PC << "\n"
                      << "SP: " << SP << "\n"
                      << "----------\n"
                      << "Z: " << Z << "\n"
                      << "S: " << S << "\n"
                      << "P: " << P << "\n"
                      << "CY: " << CY << "\n"
                      << "AC: " << AC << std::endl;
        }
};
