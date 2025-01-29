// #pragma once

// #include "RegistersBase.h"

// #include <cstdint>
// #include <iostream>

// namespace CPU_Z80 {

//     enum RegisterPairZ80 {
//         BC = 0,
//         DE = 1,
//         HL = 2,
//         SP = 3,
//         IX = 4,
//         IY = 5
//     };

//     class RegistersZ80 : public RegistersBase {
//         public:
//             Register8 A, F, B, C, D, E, H, L;
//             Register8 A_, F_, B_, C_, D_, E_, H_, L_;
//             Register16 IX, IY, PC, SP, WZ;

//             RegistersZ80() { reset(); }

//             void reset() override {
//                 A = F = B = C = D = E = H = L = 0;
//                 A_ = F_ = B_ = C_ = D_ = E_ = H_ = L_ = 0;
//                 IX = IY = PC = SP = WZ = 0;
//             }

//             uint16_t getProgramCounter() const override { return PC; }
//             void setProgramCounter(uint16_t value) override { PC = value; }

//             uint16_t getStackPointer() const override { return SP; }
//             void setStackPointer(uint16_t value) override { SP = value; }

//             uint16_t getRegisterPair(int pair) const override {
//                 switch(pair) {
//                     case RegisterPairZ80::BC: return (B << 8) | C;
//                     case RegisterPairZ80::DE: return (D << 8) | E;
//                     case RegisterPairZ80::HL: return (H << 8) | L;
//                     case RegisterPairZ80::SP: return SP;
//                     case RegisterPairZ80::IX: return IX;
//                     case RegisterPairZ80::IY: return IY;
//                     default: return 0;
//                 }
//             }

//             void setRegisterPair(int pair, uint16_t value) override {
//                 switch(pair) {
//                     case RegisterPairZ80::BC: B = (value >> 8) & 0xFF; C = value & 0xFF; break;
//                     case RegisterPairZ80::DE: D = (value >> 8) & 0xFF; E = value & 0xFF; break;
//                     case RegisterPairZ80::HL: H = (value >> 8) & 0xFF; L = value & 0xFF; break;
//                     case RegisterPairZ80::SP: SP = value; break;
//                     case RegisterPairZ80::IX: IX = value; break;
//                     case RegisterPairZ80::IY: IY = value; break;
//                 }
//             }

//             uint16_t AF() const { return (A << 8) | F; }
//             uint16_t BC() const { return (B << 8) | C; }
//             uint16_t DE() const { return (D << 8) | E; }
//             uint16_t HL() const { return (H << 8) | L; }

//             void AF(uint16_t value) { A = (value >> 8) & 0xFF; F = value & 0xFF; }
//             void BC(uint16_t value) { B = (value >> 8) & 0xFF; C = value & 0xFF; }
//             void DE(uint16_t value) { D = (value >> 8) & 0xFF; E = value & 0xFF; }
//             void HL(uint16_t value) { H = (value >> 8) & 0xFF; L = value & 0xFF; }

//             void dumpRegisters() const override {
//                 std::cout << "A: " << A << "\n"
//                         << "F: " << F << "\n"
//                         << "B: " << B << "\n"
//                         << "C: " << C << "\n"
//                         << "D: " << D << "\n"
//                         << "E: " << E << "\n"
//                         << "H: " << H << "\n"
//                         << "L: " << L << "\n"
//                         << "----------\n"
//                         << "IX: " << IX << "\n"
//                         << "IY: " << IY << "\n"
//                         << "PC: " << PC << "\n"
//                         << "SP: " << SP << "\n"
//                         << "WZ: " << WZ << "\n";
//             }
//     };

// } // namespace CPU_Z80