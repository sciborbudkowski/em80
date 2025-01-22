#pragma once

#include "RegistersBase.h"

#include <cstdint>
#include <iostream>

namespace CPU_8080 {
    enum RegisterPair8080 {
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
                switch(pair) {
                    case RegisterPair8080::BC: return (B << 8) | C;
                    case RegisterPair8080::DE: return (D << 8) | E;
                    case RegisterPair8080::HL: return (H << 8) | L;
                    case RegisterPair8080::SP: return SP;
                    default: return 0;
                }
            }

            void setRegisterPair(int pair, uint16_t value) override {
                switch(pair) {
                    case RegisterPair8080::BC: B = (value >> 8) & 0xFF; C = value & 0xFF; break;
                    case RegisterPair8080::DE: D = (value >> 8) & 0xFF; E = value & 0xFF; break;
                    case RegisterPair8080::HL: H = (value >> 8) & 0xFF; L = value & 0xFF; break;
                    case RegisterPair8080::SP: SP = value; break;
                }
            }

            uint16_t BC() const { return (B << 8) | C; }
            uint16_t DE() const { return (D << 8) | E; }
            uint16_t HL() const { return (H << 8) | L; }
            void BC(uint16_t value) { B = (value >> 8) & 0xFF; C = value & 0xFF; }
            void DE(uint16_t value) { D = (value >> 8) & 0xFF; E = value & 0xFF; }
            void HL(uint16_t value) { H = (value >> 8) & 0xFF; L = value & 0xFF; }


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
                        << "Z: " << "\n"
                        << "S: " << S << "\n"
                        << "P: " << P << "\n"
                        << "CY: " << CY << "\n"
                        << "AC: " << AC << std::endl;
            }
    };
    
} // namespace CPU_8080


/*
enum class TypeA {
    A,
    B,
    C
};

class KlasaA : public KlasaBazowa {
    public:
        uint16_t getValue(TypeA type) {
            switch(type) {
                case TypeA::A: return 1;
                case TypeA::B: return 2;
                case TypeA::C: return 3;
            }
        }
};

enum class TypeB {
    A,
    B,
    C
};

class KlasaB : public KlasaBazowa {
    public:
        uint16_t getValue(TypeB type) {
            switch(type) {
                case TypeB::A: return 1;
                case TypeB::B: return 2;
                case TypeB::C: return 3;
            }
        }
};

class KlasaBazowa {
    public:
        virtual uint16_t getValue(???) = 0;
};
*/