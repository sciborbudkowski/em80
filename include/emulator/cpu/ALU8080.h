#pragma once

#include "Registers8080.h"

#include <cstdint>

namespace CPU_8080 {

    class ALU8080 {
        public:
            static uint8_t adc(uint8_t acc, uint8_t value, Registers8080& regs) {
                uint16_t result = acc + value + (uint16_t)regs.CY;
                regs.CY = (result > 0xFF);
                uint8_t result8 = result & 0xFF;
                setFlags(result8, regs);

                return result8;
            }

            static uint8_t sbb(uint8_t acc, uint8_t value, uint8_t borrow, Registers8080& regs) {
                int result = acc - value - borrow;
                regs.CY = (result < 0);
                uint8_t result8 = result & 0xFF;
                setFlags(result8, regs);
                regs.AC = ((acc & 0xF) < ((value & 0xF) + borrow));

                return result8;
            }

            static void cmp(uint8_t acc, uint8_t value, Registers8080& regs) {
                int result = acc - value;
                regs.CY = (result < 0);
                uint8_t result8 = result & 0xFF;
                setFlags(result8, regs);
                regs.AC = ((acc & 0xF) < (value & 0xF));
            }

            static void setFlags(uint8_t result, Registers8080& regs) {
                regs.Z = (result == 0);             // Zero flag
                regs.S = (result & 0x80);           // Sign flag
                regs.P = calculateParity(result);   // Parity flag
            }

            static bool calculateParity(uint8_t value) {
                int parity = 0;
                for(int i=0; i<8; i++) {
                    parity ^= (value >> 1) & 1;
                }

                return (parity == 0);
            }
    };

} // namespace CPU_8080