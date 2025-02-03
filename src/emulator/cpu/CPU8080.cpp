#include "CPU8080.h"
#include "ALU8080.h"
#include "Memory8080.h"
#include "IO8080.h"
#include "Terminal8080.h"
#include "CPUUtils.h"
#include "helpers/BusyWait.h"

#include <chrono>
#include <thread>
#include <map>
#include <sstream>
#include <iomanip>
#include <iostream>

void CPU8080::reset() {
    const char* welcomeText = "Intel 8080 Emulator.";

    regs.reset();
    memory->clear();
    setRunning(true);
    turboMode = false;
    interruptsEnabled = false;
    io->terminal->clear();
    io->terminal->printString(welcomeText);
    io->terminal->setCursorPos(0, 2);
    io->terminal->setLastCommand("");

    currentTimer = 0;
    executedInstructions = 0;
}

void CPU8080::pushStack(uint16_t value) {
    memory->write(--regs.SP, (value >> 8) & 0xFF);
    memory->write(--regs.SP, value & 0xFF);
}

uint16_t CPU8080::popStack() {
    uint16_t value = memory->read(regs.SP) << 8 | memory->read(regs.SP + 1);
    regs.SP += 2;
    return value;
}

void CPU8080::step() {
    if (!isRunning()) return;

    uint8_t opcode = memory->read(regs.PC++);
    decodeAndExecute(opcode);
}

void CPU8080::run() {
    while (isRunning()) {
        step();
    }
}

void CPU8080::decodeAndExecute(uint8_t opcode) {
    uint16_t addr;
    uint8_t value, result, port;

    //double cycleDuration = (1.0 / clockSpeed) * instructionCycles[opcode] * 1000000.0;
    int cycleDuration = instructionTimes[opcode];
    auto startTime = std::chrono::high_resolution_clock::now();

    std::string asmInstr;

    switch(opcode) {
        // --- NOP ---
        case 0x00: asmInstr = "NOP"; break;

        // --- MOV r1, r2 ---
        case 0x78: regs.A = regs.B; asmInstr = "MOV A, B"; break;
        case 0x79: regs.A = regs.C; asmInstr = "MOV A, C"; break;
        case 0x7A: regs.A = regs.D; asmInstr = "MOV A, D"; break;
        case 0x7B: regs.A = regs.E; asmInstr = "MOV A, E"; break;
        case 0x7C: regs.A = regs.H; asmInstr = "MOV A, H"; break;
        case 0x7D: regs.A = regs.L; asmInstr = "MOV A, L"; break;

        case 0x7E:
            addr = memory->read((regs.H << 8) | regs.L);
            regs.A = addr;
            asmInstr = "MOV A, [" + utils.hex16_t[addr] + "]";
            break;

        case 0x7F: 
            regs.A = regs.A; 
            asmInstr = "MOV A, A"; 
            break;

        case 0x40: regs.B = regs.B; asmInstr = "MOV B, B"; break;
        case 0x41: regs.B = regs.C; asmInstr = "MOV B, C"; break;
        case 0x42: regs.B = regs.D; asmInstr = "MOV B, D"; break;
        case 0x43: regs.B = regs.E; asmInstr = "MOV B, E"; break;
        case 0x44: regs.B = regs.H; asmInstr = "MOV B, H"; break;
        case 0x45: regs.B = regs.L; asmInstr = "MOV B, L"; break;
        case 0x46: regs.B = memory->read((regs.H << 8) | regs.L); asmInstr = "MOV B, M"; break;
        case 0x47: regs.B = regs.A; asmInstr = "MOV B, A"; break;

        case 0x48: regs.C = regs.B; asmInstr = "MOV C, B"; break;
        case 0x49: regs.C = regs.C; asmInstr = "MOV C, C"; break;
        case 0x4A: regs.C = regs.D; asmInstr = "MOV C, D"; break;
        case 0x4B: regs.C = regs.E; asmInstr = "MOV C, E"; break;
        case 0x4C: regs.C = regs.H; asmInstr = "MOV C, H"; break;
        case 0x4D: regs.C = regs.L; asmInstr = "MOV C, L"; break;
        case 0x4E: regs.C = memory->read((regs.H << 8) | regs.L); asmInstr = "MOV C, M"; break;
        case 0x4F: regs.C = regs.A; asmInstr = "MOV C, A"; break;

        // --- MVI r, data ---
        case 0x06: regs.B = memory->read(regs.PC++); asmInstr = "MVI B, " + utils.hex8_t[regs.B]; break;
        case 0x0E: regs.C = memory->read(regs.PC++); asmInstr = "MVI C, " + utils.hex8_t[regs.C]; break;
        case 0x16: regs.D = memory->read(regs.PC++); asmInstr = "MVI D, " + utils.hex8_t[regs.D]; break;
        case 0x1E: regs.E = memory->read(regs.PC++); asmInstr = "MVI E, " + utils.hex8_t[regs.E]; break;
        case 0x26: regs.H = memory->read(regs.PC++); asmInstr = "MVI H, " + utils.hex8_t[regs.H]; break;
        case 0x2E: regs.L = memory->read(regs.PC++); asmInstr = "MVI L, " + utils.hex8_t[regs.L]; break;
        case 0x3E: regs.A = memory->read(regs.PC++); asmInstr = "MVI A, " + utils.hex8_t[regs.A]; break;

        // --- ADD A, r ---
        case 0x80: ALU8080::add(regs.A, regs.B, regs.Flags); asmInstr = "ADD B"; break;
        case 0x81: ALU8080::add(regs.A, regs.C, regs.Flags); asmInstr = "ADD C"; break;
        case 0x82: ALU8080::add(regs.A, regs.D, regs.Flags); asmInstr = "ADD D"; break;
        case 0x83: ALU8080::add(regs.A, regs.E, regs.Flags); asmInstr = "ADD E"; break;
        case 0x84: ALU8080::add(regs.A, regs.H, regs.Flags); asmInstr = "ADD H"; break;
        case 0x85: ALU8080::add(regs.A, regs.L, regs.Flags); asmInstr = "ADD L"; break;
        case 0x86: ALU8080::add(regs.A, memory->read((regs.H << 8) | regs.L), regs.Flags); asmInstr = "ADD M"; break;
        case 0x87: ALU8080::add(regs.A, regs.A, regs.Flags); asmInstr = "ADD A"; break;

        // --- SUB A, r ---
        case 0x90: ALU8080::sub(regs.A, regs.B, regs.Flags); asmInstr = "SUB B"; break;
        case 0x91: ALU8080::sub(regs.A, regs.C, regs.Flags); asmInstr = "SUB C"; break;
        case 0x92: ALU8080::sub(regs.A, regs.D, regs.Flags); asmInstr = "SUB D"; break;
        case 0x93: ALU8080::sub(regs.A, regs.E, regs.Flags); asmInstr = "SUB E"; break;
        case 0x94: ALU8080::sub(regs.A, regs.H, regs.Flags); asmInstr = "SUB H"; break;
        case 0x95: ALU8080::sub(regs.A, regs.L, regs.Flags); asmInstr = "SUB L"; break;
        case 0x96: ALU8080::sub(regs.A, memory->read((regs.H << 8) | regs.L), regs.Flags); asmInstr = "SUB M"; break;
        case 0x97: ALU8080::sub(regs.A, regs.A, regs.Flags); asmInstr = "SUB A"; break;

        // --- INR r ---
        case 0x04: ALU8080::inc(regs.B, regs.Flags); asmInstr = "INR B"; break;
        case 0x0C: ALU8080::inc(regs.C, regs.Flags); asmInstr = "INR C"; break;
        case 0x14: ALU8080::inc(regs.D, regs.Flags); asmInstr = "INR D"; break;
        case 0x1C: ALU8080::inc(regs.E, regs.Flags); asmInstr = "INR E"; break;
        case 0x24: ALU8080::inc(regs.H, regs.Flags); asmInstr = "INR H"; break;
        case 0x2C: ALU8080::inc(regs.L, regs.Flags); asmInstr = "INR L"; break;
        case 0x3C: ALU8080::inc(regs.A, regs.Flags); asmInstr = "INR A"; break;

        // --- JMP addr ---
        case 0xC3: 
            regs.PC = memory->read(regs.PC) | (memory->read(regs.PC+1) << 8);
            asmInstr = "JMP " + utils.hex16_t[regs.PC];
            break;

        // --- PUSH rp ---
        case 0xC5: pushStack((regs.B << 8) | regs.C); asmInstr = "PUSH B"; break;
        case 0xD5: pushStack((regs.D << 8) | regs.E); asmInstr = "PUSH D"; break;
        case 0xE5: pushStack((regs.H << 8) | regs.L); asmInstr = "PUSH H"; break;
        case 0xF5: {
            uint8_t flags = regs.Flags | 0b00000010;
            pushStack((regs.A << 8) | flags);
            asmInstr = "PUSH PSW";
            break;
        }

        // --- POP rp ---
        case 0xC1:
            {
                uint16_t value = popStack();
                regs.B = (value >> 8) & 0xFF;
                regs.C = value & 0xFF;
                asmInstr = "POP B";
            }
            break;
        case 0xD1:
            {
                uint16_t value = popStack();
                regs.D = (value >> 8) & 0xFF;
                regs.E = value & 0xFF;
                asmInstr = "POP D";
            }
            break;
        case 0xE1:
            {
                uint16_t value = popStack();
                regs.H = (value >> 8) & 0xFF;
                regs.L = value & 0xFF;
                asmInstr = "POP H";
            }
            break;
        case 0xF1: // POP PSW
            {
                uint16_t value = popStack();
                regs.setFlag(Registers8080::AUX_CARRY, (value >> 8) & 0xFF);
                regs.setFlag(Registers8080::ZERO, (value >> 6) & 1);
                regs.setFlag(Registers8080::SIGN, (value >> 7) & 1);
                regs.setFlag(Registers8080::PARITY, (value >> 2) & 1);
                regs.setFlag(Registers8080::CARRY, value & 1);
                asmInstr = "POP PSW";
            }
            break;

        // --- CALL addr ---
        case 0xCD:
            {
                uint16_t addr = memory->read(regs.PC) | (memory->read(regs.PC+1) << 8);
                regs.PC += 2;

                if(addr == CBIOS) {
                    if(cpmbios) {
                    cpmbios->call(regs.C);
                    } else {
                        throw std::runtime_error("CPM BIOS not initialized!");
                    }
                    asmInstr = "CALL BIOS " + utils.hex8_t[regs.C];
                } else {
                    pushStack(regs.PC);
                    regs.PC = addr;
                    asmInstr = "CALL " + utils.hex16_t[addr];
                }
            }
            break;

        // --- RET ---
        case 0xC9: regs.PC = popStack(); asmInstr = "RET"; break;

        // --- conditional jumps ---
        case 0xCA: // JZ addr
            addr = memory->read(regs.PC) | (memory->read(regs.PC+1) << 8);
            regs.PC += 2;
            if(regs.getFlag(Registers8080::ZERO)) {
                regs.PC = addr;
            }
            asmInstr = "JZ " + utils.hex16_t[addr];
            break;

        case 0xC2: // JNZ addr
            addr = memory->read(regs.PC) | (memory->read(regs.PC+1) << 8);
            if(!regs.getFlag(Registers8080::ZERO)) {
                regs.PC = addr;
            } else {
                regs.PC += 2;
            }
            asmInstr = "JNZ " + utils.hex16_t[addr];
            break;

        case 0xD2: // JNC addr
            addr = memory->read(regs.PC) | (memory->read(regs.PC+1) << 8);
            if(!regs.getFlag(Registers8080::CARRY)) {
                regs.PC = addr;
            } else {
                regs.PC += 2;
            }
            asmInstr = "JNC " + utils.hex16_t[addr];
            break;

        case 0xDA: // JC addr
            addr = memory->read(regs.PC) | (memory->read(regs.PC+1) << 8);
            if(regs.getFlag(Registers8080::CARRY)) {
                regs.PC = addr;
            } else {
                regs.PC += 2;
            }
            asmInstr = "JC " + utils.hex16_t[addr];
            break;

        case 0xE2: // JPO addr
            addr = memory->read(regs.PC) | (memory->read(regs.PC+1) << 8);
            if(!regs.getFlag(Registers8080::PARITY)) {
                regs.PC = addr;
            } else {
                regs.PC += 2;
            }
            asmInstr = "JPO " + utils.hex16_t[addr];
            break;

        case 0xEA: // JPE addr
            addr = memory->read(regs.PC) | (memory->read(regs.PC+1) << 8);
            if(regs.getFlag(Registers8080::PARITY)) {
                regs.PC = addr;
            } else {
                regs.PC += 2;
            }
            asmInstr = "JPE " + utils.hex16_t[addr];
            break;

        case 0xFA: // JM addr
            addr = memory->read(regs.PC) | (memory->read(regs.PC+1) << 8);
            if(regs.getFlag(Registers8080::SIGN)) {
                regs.PC = addr;
            } else {
                regs.PC += 2;
            }
            asmInstr = "JM " + utils.hex16_t[addr];
            break;

        // ---conditional calls ---
        case 0xCC: // CZ
            addr = memory->read(regs.PC) | (memory->read(regs.PC+1) << 8);
            if(regs.getFlag(Registers8080::ZERO)) {
                regs.PC += 2;
                pushStack(regs.PC);
                regs.PC = addr;
            } else {
                regs.PC += 2;
            }
            asmInstr = "CZ " + utils.hex16_t[addr];
            break;

        case 0xC4: // CNZ
            addr = memory->read(regs.PC) | (memory->read(regs.PC+1) << 8);
            if(!regs.getFlag(Registers8080::ZERO)) {
                regs.PC += 2;
                pushStack(regs.PC);
                regs.PC = addr;
            } else {
                regs.PC += 2;
            }
            asmInstr = "CNZ " + utils.hex16_t[addr];
            break;

        case 0xDC: // CC
            addr = memory->read(regs.PC) | (memory->read(regs.PC+1) << 8);
            if(regs.getFlag(Registers8080::CARRY)) {
                regs.PC += 2;
                pushStack(regs.PC);
                regs.PC = addr;
            } else {
                regs.PC += 2;
            }
            asmInstr = "CC " + utils.hex16_t[addr];
            break;

        case 0xD4: // CNC
            addr = memory->read(regs.PC) | (memory->read(regs.PC+1) << 8);
            if(!regs.getFlag(Registers8080::CARRY)) {
                regs.PC += 2;
                pushStack(regs.PC);
                regs.PC = addr;
            } else {
                regs.PC += 2;
            }
            asmInstr = "CNC " + utils.hex16_t[addr];
            break;

        case 0xEC: // CPE
            addr = memory->read(regs.PC) | (memory->read(regs.PC+1) << 8);
            if(regs.getFlag(Registers8080::PARITY)) {
                regs.PC += 2;
                pushStack(regs.PC);
                regs.PC = addr;
            } else {
                regs.PC += 2;
            }
            asmInstr = "CPE " + utils.hex16_t[addr];
            break;

        case 0xE4: // CPO
            addr = memory->read(regs.PC) | (memory->read(regs.PC+1) << 8);
            if(!regs.getFlag(Registers8080::PARITY)) {
                regs.PC += 2;
                pushStack(regs.PC);
                regs.PC = addr;
            } else {
                regs.PC += 2;
            }
            asmInstr = "CPO " + utils.hex16_t[addr];
            break;

        case 0xF4: // CP
            addr = memory->read(regs.PC) | (memory->read(regs.PC+1) << 8);
            if(!regs.getFlag(Registers8080::SIGN)) {
                regs.PC += 2;
                pushStack(regs.PC);
                regs.PC = addr;
            } else {
                regs.PC += 2;
            }
            asmInstr = "CP " + utils.hex16_t[addr];
            break;

        case 0xFC: // CM
            addr = memory->read(regs.PC) | (memory->read(regs.PC+1) << 8);
            if(regs.getFlag(Registers8080::SIGN)) {
                regs.PC += 2;
                pushStack(regs.PC);
                regs.PC = addr;
            } else {
                regs.PC += 2;
            }
            asmInstr = "CM " + utils.hex16_t[addr];
            break;

        // --- AND (ANA) ---
        case 0xA0: regs.B = ALU8080::and_op(regs.A, regs.B, regs.Flags); asmInstr = "ANA B"; break;
        case 0xA1: regs.C = ALU8080::and_op(regs.A, regs.C, regs.Flags); asmInstr = "ANA C"; break;
        case 0xA2: regs.D = ALU8080::and_op(regs.A, regs.D, regs.Flags); asmInstr = "ANA D"; break;
        case 0xA3: regs.E = ALU8080::and_op(regs.A, regs.E, regs.Flags); asmInstr = "ANA E"; break;
        case 0xA4: regs.H = ALU8080::and_op(regs.A, regs.H, regs.Flags); asmInstr = "ANA H"; break;
        case 0xA5: regs.L = ALU8080::and_op(regs.A, regs.L, regs.Flags); asmInstr = "ANA L"; break;
        case 0xA6: regs.A = ALU8080::and_op(regs.A, memory->read((regs.H << 8) | regs.L), regs.Flags); asmInstr = "ANA M"; break;
        case 0xA7: regs.A = ALU8080::and_op(regs.A, regs.A, regs.Flags); asmInstr = "ANA A"; break;

        // -- AND immediate (ANI) ---
        case 0xE6:
            value = memory->read(regs.PC++);
            regs.A &= value;
            ALU8080::and_op(regs.A, value, regs.Flags);
            asmInstr = "ANI " + utils.hex8_t[value];
            break;

        // --- XOR (XRA) ---
        case 0xA8: regs.B = ALU8080::xor_op(regs.A, regs.B, regs.Flags); asmInstr = "XRA B"; break;
        case 0xA9: regs.C = ALU8080::xor_op(regs.A, regs.C, regs.Flags); asmInstr = "XRA C"; break;
        case 0xAA: regs.D = ALU8080::xor_op(regs.A, regs.D, regs.Flags); asmInstr = "XRA D"; break;
        case 0xAB: regs.E = ALU8080::xor_op(regs.A, regs.E, regs.Flags); asmInstr = "XRA E"; break;
        case 0xAC: regs.H = ALU8080::xor_op(regs.A, regs.H, regs.Flags); asmInstr = "XRA H"; break;
        case 0xAD: regs.L = ALU8080::xor_op(regs.A, regs.L, regs.Flags); asmInstr = "XRA L"; break;
        case 0xAE: regs.A = ALU8080::xor_op(regs.A, memory->read((regs.H << 8) | regs.L), regs.Flags); asmInstr = "XRA M"; break;
        case 0xAF: regs.A = ALU8080::xor_op(regs.A, regs.A, regs.Flags); asmInstr = "XRA A"; break;

        // --- XOR immediate (XRI) ---
        case 0xEE:
            value = memory->read(regs.PC++);
            regs.A ^= value;
            ALU8080::xor_op(regs.A, value, regs.Flags);
            asmInstr = "XRI " + utils.hex8_t[value];
            break;

        // --- OR (ORA) ---
        case 0xB0: regs.B = ALU8080::or_op(regs.A, regs.B, regs.Flags); asmInstr = "ORA B"; break;
        case 0xB1: regs.C = ALU8080::or_op(regs.A, regs.C, regs.Flags); asmInstr = "ORA C"; break;
        case 0xB2: regs.D = ALU8080::or_op(regs.A, regs.D, regs.Flags); asmInstr = "ORA D"; break;
        case 0xB3: regs.E = ALU8080::or_op(regs.A, regs.E, regs.Flags); asmInstr = "ORA E"; break;
        case 0xB4: regs.H = ALU8080::or_op(regs.A, regs.H, regs.Flags); asmInstr = "ORA H"; break;
        case 0xB5: regs.L = ALU8080::or_op(regs.A, regs.L, regs.Flags); asmInstr = "ORA L"; break;
        case 0xB6: regs.A = ALU8080::or_op(regs.A, memory->read((regs.H << 8) | regs.L), regs.Flags); asmInstr = "ORA M"; break;
        case 0xB7: regs.A = ALU8080::or_op(regs.A, regs.A, regs.Flags); asmInstr = "ORA A"; break;

        // --- OR immediate (ORI) ---
        case 0xF6:
            value = memory->read(regs.PC++);
            regs.A |= value;
            ALU8080::or_op(regs.A, value, regs.Flags);
            asmInstr = "ORI " + utils.hex8_t[value];
            break;

        // --- CPI (Compare A with immediate) ---
        case 0xFE: {
            uint8_t data = memory->read(regs.PC++);
            uint8_t flags = regs.Flags;
            ALU8080::cmp(regs.A, data, flags);
            regs.Flags = flags;
            asmInstr = "CPI " + utils.hex8_t[data];
            break;
        }

        // --- IN port ---
        case 0xDB:
            port = memory->read(regs.PC++);
            value = io->in(port);
            regs.A = value;
            asmInstr = "IN " + utils.hex8_t[port];
            break;

        // --- OUT port ---
        case 0xD3:
            port = memory->read(regs.PC++);
            io->out(port, regs.A);
            asmInstr = "OUT " + utils.hex8_t[port];
            break;

        // --- HLT ---
        case 0x76:
            setRunning(false); 
            asmInstr = "HLT"; 
            std::cout << "Number of executed instructions: " << getExecutedInstructions() << std::endl; 
            std::cout << "Total speeding: " << totalSpeeding << "us" << std::endl;
            break;

        // --- LXI B, d16 ---
        case 0x01:
            regs.C = memory->read(regs.PC++);
            regs.B = memory->read(regs.PC++);
            asmInstr = "LXI B, " + utils.hex16_t[(regs.B << 8) | regs.C];
            break;

        // --- STAX B ---
        case 0x02: memory->write((regs.B << 8) | regs.C, regs.A); asmInstr = "STAX B"; break;

        // --- INX B ---
        case 0x03:
            regs.C++;
            if(regs.C == 0) regs.B++;
            asmInstr = "INX B";
            break;

        // --- DCR B ---
        case 0x05: ALU8080::dec(regs.B, regs.Flags); asmInstr = "DCR B"; break;

        // -- RLC --
        case 0x07:
            regs.setFlag(Registers8080::CARRY, (regs.A & 0x80) != 0);
            regs.A = (regs.A << 1) | regs.getFlag(Registers8080::CARRY);
            asmInstr = "RLC";
            break;

        // --- DAD B (HL += BC) ---
        case 0x09:
            {
                uint32_t result = regs.HL() + regs.BC();
                regs.H = (result >> 8) & 0xFF;
                regs.L = result & 0xFF;
                regs.setFlag(Registers8080::CARRY, result > 0xFFFF);
                asmInstr = "DAD B";
            }
            break;

        // --- LDAX B ---
        case 0x0A: regs.A = memory->read((regs.B << 8) | regs.C); asmInstr = "LDAX B"; break;

        // --- DCX B ---
        case 0x0B:
            regs.BC(regs.BC() - 1);
            asmInstr = "DCX B";
            break;

        // --- DCR C ---
        case 0x0D: {
            uint8_t flags = regs.Flags;
            regs.C = ALU8080::dec(regs.C, flags);
            regs.Flags = flags;
            asmInstr = "DCR C";
            break;
        }

        // --- RRC ---
        case 0x0F:
            regs.setFlag(Registers8080::CARRY, (regs.A & 1) != 0);
            regs.A = (regs.A >> 1) | (regs.getFlag(Registers8080::CARRY) << 7);
            asmInstr = "RRC";
            break;

        // --- LXI D, d16 ---
        case 0x11:
            regs.E = memory->read(regs.PC++);
            regs.D = memory->read(regs.PC++);
            asmInstr = "LXI D, " + utils.hex16_t[(regs.D << 8) | regs.E];
            break;

        // --- STAX D ---
        case 0x12: memory->write((regs.D << 8) | regs.E, regs.A); asmInstr = "STAX D"; break;

        // --- INX D ---
        case 0x13:
            regs.E++;
            if(regs.E == 0) regs.D++;
            asmInstr = "INX D";
            break;

        // --- DCR D ---
        case 0x15:
            regs.D--;
            ALU8080::dec(regs.D, regs.Flags);
            asmInstr = "DCR D";
            break;

        // --- RAL ---
        case 0x17:
            {
                uint8_t oldCY = regs.getFlag(Registers8080::CARRY);
                regs.setFlag(Registers8080::CARRY, (regs.A & 0x80) != 0);
                regs.A = (regs.A << 1) | oldCY;
                asmInstr = "RAL";
            }
            break;

        // --- DAD D ---
        case 0x19:
            {
                uint32_t result = regs.HL() + regs.DE();
                regs.H = (result >> 8) & 0xFF;
                regs.L = result & 0xFF;
                regs.setFlag(Registers8080::CARRY, result > 0XFFFF);
                asmInstr = "DAD D";
            }
            break;

        // --- LDAX D ---
        case 0x1A: regs.A = memory->read((regs.D << 8) | regs.E); asmInstr = "LDAX D"; break;

        // --- DCX D ---
        case 0x1B:
            regs.E--;
            if(regs.E == 0xFF) regs.D--;
            asmInstr = "DCX D";
            break;

        // --- DCR E ---
        case 0x1D:
            regs.E--;
            ALU8080::dec(regs.E, regs.Flags);
            asmInstr = "DCR E";
            break;

        // --- RAR ---
        case 0x1F:
            {
                uint8_t oldCY = regs.getFlag(Registers8080::CARRY);
                regs.setFlag(Registers8080::CARRY, (regs.A & 1) != 0);
                regs.A = (regs.A >> 1) | (oldCY << 7);
                asmInstr = "RAR";
            }
            break;

        // --- LXI H, d16 ---
        case 0x21:
            regs.L = memory->read(regs.PC);
            regs.H = memory->read(regs.PC+1);
            regs.PC += 2;
            asmInstr = "LXI H, " + utils.hex16_t[(regs.H << 8) | regs.L];
            break;

        // --- SHLD addr ---
        case 0x22:
            {
                uint8_t low = memory->read(regs.PC++);
                uint8_t high = memory->read(regs.PC++);
                uint16_t addr = (high << 8) | low;
                memory->write(addr, regs.L);
                memory->write(addr+1, regs.H);
                asmInstr = "SHLD " + utils.hex16_t[addr];
            }
            break;

        // --- INX H ---
        case 0x23:
            regs.L++;
            if(regs.L == 0) regs.H++;
            asmInstr = "INX H";
            break;

        // --- DRC H ---
        case 0x25:
            regs.H--;
            ALU8080::dec(regs.H, regs.Flags);
            asmInstr = "DCR H";
            break;

        // --- DAA ---
        case 0x27:
            {
                if((regs.A & 0x0F) > 9 || regs.getFlag(Registers8080::AUX_CARRY)) {
                    regs.A += 6;
                    regs.setFlag(Registers8080::AUX_CARRY, 1);
                }
                if((regs.A >> 4) > 9 || regs.getFlag(Registers8080::CARRY)) {
                    regs.A += 0x60;
                    regs.setFlag(Registers8080::CARRY, 1);
                }
                asmInstr = "DAA";
            }
            break;

        case 0x50: regs.D = regs.B; asmInstr = "MOV D, B"; break;
        case 0x51: regs.D = regs.C; asmInstr = "MOV D, C"; break;
        case 0x52: regs.D = regs.D; asmInstr = "MOV D, D"; break;
        case 0x53: regs.D = regs.E; asmInstr = "MOV D, E"; break;
        case 0x54: regs.D = regs.H; asmInstr = "MOV D, H"; break;
        case 0x55: regs.D = regs.L; asmInstr = "MOV D, L"; break;
        case 0x56: regs.D = memory->read((regs.H << 8) | regs.L); asmInstr = "MOV D, M"; break;
        case 0x57: regs.D = regs.A; asmInstr = "MOV D, A"; break;

        case 0x58: regs.E = regs.B; asmInstr = "MOV E, B"; break;
        case 0x59: regs.E = regs.C; asmInstr = "MOV E, C"; break;
        case 0x5A: regs.E = regs.D; asmInstr = "MOV E, D"; break;
        case 0x5B: regs.E = regs.E; asmInstr = "MOV E, E"; break;
        case 0x5C: regs.E = regs.H; asmInstr = "MOV E, H"; break;
        case 0x5D: regs.E = regs.L; asmInstr = "MOV E, L"; break;
        case 0x5E: regs.E = memory->read((regs.H << 8) | regs.L); asmInstr = "MOV E, M"; break;
        case 0x5F: regs.E = regs.A; asmInstr = "MOV E, A"; break;

        case 0x60: regs.H = regs.B; asmInstr = "MOV H, B"; break;
        case 0x61: regs.H = regs.C; asmInstr = "MOV H, C"; break;
        case 0x62: regs.H = regs.D; asmInstr = "MOV H, D"; break;
        case 0x63: regs.H = regs.E; asmInstr = "MOV H, E"; break;
        case 0x64: regs.H = regs.H; asmInstr = "MOV H, H"; break;
        case 0x65: regs.H = regs.L; asmInstr = "MOV H, L"; break;
        case 0x66: regs.H = memory->read((regs.H << 8) | regs.L); asmInstr = "MOV H, M"; break;
        case 0x67: regs.H = regs.A; asmInstr = "MOV H, A"; break;

        case 0x68: regs.L = regs.B; asmInstr = "MOV L, B"; break;
        case 0x69: regs.L = regs.C; asmInstr = "MOV L, C"; break;
        case 0x6A: regs.L = regs.D; asmInstr = "MOV L, D"; break;
        case 0x6B: regs.L = regs.E; asmInstr = "MOV L, E"; break;
        case 0x6C: regs.L = regs.H; asmInstr = "MOV L, H"; break;
        case 0x6D: regs.L = regs.L; asmInstr = "MOV L, L"; break;
        case 0x6E: regs.L = memory->read((regs.H << 8) | regs.L); asmInstr = "MOV L, M"; break;
        case 0x6F: regs.L = regs.A; asmInstr = "MOV L, A"; break;

        case 0x70: memory->write((regs.H << 8) | regs.L, regs.B); asmInstr = "MOV M, B"; break;
        case 0x71: memory->write((regs.H << 8) | regs.L, regs.C); asmInstr = "MOV M, C"; break;
        case 0x72: memory->write((regs.H << 8) | regs.L, regs.D); asmInstr = "MOV M, D"; break;
        case 0x73: memory->write((regs.H << 8) | regs.L, regs.E); asmInstr = "MOV M, E"; break;
        case 0x74: memory->write((regs.H << 8) | regs.L, regs.H); asmInstr = "MOV M, H"; break;
        case 0x75: memory->write((regs.H << 8) | regs.L, regs.L); asmInstr = "MOV M, L"; break;

        case 0x28: asmInstr = "NOP (reserved)"; break;
        case 0x30: asmInstr = "NOP (reserved)"; break;
        case 0x38: asmInstr = "NOP (reserved)"; break;
        case 0x08: asmInstr = "NOP (reserved)"; break;
        case 0x10: asmInstr = "NOP (reserved)"; break;
        case 0x18: asmInstr = "NOP (reserved)"; break;
        case 0x20: asmInstr = "NOP (reserved)"; break;

        // --- DAD H (HL = HL + HL)
        case 0x29:
            {
                uint32_t result = regs.HL() + regs.HL();
                regs.H = (result >> 8) & 0xFF;
                regs.L = result & 0xFF;
                regs.setFlag(Registers8080::CARRY, result > 0xFFFF);
                asmInstr = "DAD H";
            }
            break;

        // --- LHLD addr ---
        case 0x2a:
            {
                uint8_t low = memory->read(regs.PC++);
                uint8_t high = memory->read(regs.PC++);
                uint16_t addr =  (high << 8) | low;
                regs.L = memory->read(addr);
                regs.H = memory->read(addr+1);
                asmInstr = "LHLD " + utils.hex16_t[addr];
            }
            break;

        // --- DCX H (HL = HL - ) ---
        case 0x2b:
            if(regs.L == 0) regs.H--;
            regs.L--;
            asmInstr = "DCX H";
            break;

        // --- DCR L ---
        case 0x2d:
            regs.L--;
            ALU8080::dec(regs.L, regs.Flags);
            asmInstr = "DCR L";
            break;

        // --- CMA (A = ~A)
        case 0x2f: regs.A = ~regs.A; asmInstr = "CMA"; break;

        // --- LXI SP, d16 ---
        case 0x31:
            {
                uint8_t low = memory->read(regs.PC++);
                uint8_t high = memory->read(regs.PC++);
                regs.SP = (high << 8) | low;
                asmInstr = "LXI SP, " + utils.hex16_t[regs.SP];
            }
            break;

        // --- STA addr ---
        case 0x32:
            {
                uint8_t low = memory->read(regs.PC++);
                uint8_t high = memory->read(regs.PC++);
                uint16_t addr =  (high << 8) | low;
                memory->write(addr, regs.A);
                asmInstr = "STA " + utils.hex16_t[addr];
            }
            break;

        // --- INX SP ---
        case 0x33: regs.SP++; asmInstr = "INX SP"; break;

        // --- INR M ---
        case 0x34: {
                uint16_t addr = regs.HL();
                uint8_t value = memory->read(addr);
                value = ALU8080::inc(value, regs.Flags);
                memory->write(addr, value);
                asmInstr = "INR M";
            }
            break;

        // --- DCR M ---
        case 0x35:
            {
                uint16_t addr = regs.HL();
                uint8_t value = memory->read(addr);
                memory->write(addr, value--);
                ALU8080::dec(value, regs.Flags);
                asmInstr = "DCR M";
            }
            break;

        // --- MVI M, d8 ---
        case 0x36:
            {
                uint8_t value = memory->read(regs.PC++);
                memory->write(regs.HL(), value);
                asmInstr = "MVI M, " + utils.hex8_t[value];
            }
            break;

        // --- STC ---
        case 0x37: regs.setFlag(Registers8080::CARRY, 1); asmInstr = "STC"; break;

        // --- DAD SP ---
        case 0x39:
            {
                uint32_t result = regs.HL() + regs.SP;
                regs.H = (result >> 8) & 0xFF;
                regs.L = result & 0xFF;
                regs.setFlag(Registers8080::CARRY, result > 0xFFFF);
                asmInstr = "DAD SP";
            }
            break;

        // --- LDA addr ---
        case 0x3a:
            {
                uint8_t low = memory->read(regs.PC++);
                uint8_t high = memory->read(regs.PC++);
                uint16_t addr =  (high << 8) | low;
                regs.A = memory->read(addr);
                asmInstr = "LDA " + utils.hex16_t[addr];
            }
            break;

        // --- DCX SP ---
        case 0x3b: regs.SP--; asmInstr = "DCX SP"; break;

        // --- DCR A ---
        case 0x3d: regs.A--; ALU8080::dec(regs.A, regs.Flags); asmInstr = "DCR A"; break;

        // --- CMC ---
        case 0x3f: regs.setFlag(Registers8080::CARRY, !regs.getFlag(Registers8080::CARRY)); asmInstr = "CMC"; break;

        // --- MOV M, A ---
        case 0x77: memory->write((regs.H << 8) | regs.L, regs.A); asmInstr = "MOV M, A"; break;

        // --- ADC reg ---
        case 0x88: regs.A = ALU8080::adc(regs.A, regs.B, regs.Flags); asmInstr = "ADC B"; break;
        case 0x89: regs.A = ALU8080::adc(regs.A, regs.C, regs.Flags); asmInstr = "ADC C"; break;
        case 0x8a: regs.A = ALU8080::adc(regs.A, regs.D, regs.Flags); asmInstr = "ADC D"; break;
        case 0x8b: regs.A = ALU8080::adc(regs.A, regs.E, regs.Flags); asmInstr = "ADC E"; break;
        case 0x8c: regs.A = ALU8080::adc(regs.A, regs.H, regs.Flags); asmInstr = "ADC H"; break;
        case 0x8d: regs.A = ALU8080::adc(regs.A, regs.L, regs.Flags); asmInstr = "ADC L"; break;
        case 0x8e: regs.A = ALU8080::adc(regs.A, memory->read((regs.H << 8) | regs.L), regs.Flags); asmInstr = "ADC M"; break;
        case 0x8f: regs.A = ALU8080::adc(regs.A, regs.A, regs.Flags); asmInstr = "ADC A"; break;

        // --- SBB reg ---
        case 0x98: regs.A = ALU8080::sbb(regs.A, regs.B, regs.Flags); asmInstr = "SBB B"; break;
        case 0x99: regs.A = ALU8080::sbb(regs.A, regs.C, regs.Flags); asmInstr = "SBB C"; break;
        case 0x9a: regs.A = ALU8080::sbb(regs.A, regs.D, regs.Flags); asmInstr = "SBB D"; break;
        case 0x9b: regs.A = ALU8080::sbb(regs.A, regs.E, regs.Flags); asmInstr = "SBB E"; break;
        case 0x9c: regs.A = ALU8080::sbb(regs.A, regs.H, regs.Flags); asmInstr = "SBB H"; break;
        case 0x9d: regs.A = ALU8080::sbb(regs.A, regs.L, regs.Flags); asmInstr = "SBB L"; break;
        case 0x9e: regs.A = ALU8080::sbb(regs.A, memory->read(regs.HL()), regs.Flags); asmInstr = "SBB M"; break;
        case 0x9f: regs.A = ALU8080::sbb(regs.A, regs.A, regs.Flags); asmInstr = "SBB A"; break;

        // --- CMP reg ---
        case 0xB8: ALU8080::cmp(regs.A, regs.B, regs.Flags); asmInstr = "CMP B"; break;
        case 0xB9: ALU8080::cmp(regs.A, regs.C, regs.Flags); asmInstr = "CMP C"; break;
        case 0xBA: ALU8080::cmp(regs.A, regs.D, regs.Flags); asmInstr = "CMP D"; break;
        case 0xBB: ALU8080::cmp(regs.A, regs.E, regs.Flags); asmInstr = "CMP E"; break;
        case 0xBC: ALU8080::cmp(regs.A, regs.H, regs.Flags); asmInstr = "CMP H"; break;
        case 0xBD: ALU8080::cmp(regs.A, regs.L, regs.Flags); asmInstr = "CMP L"; break;
        case 0xBE: ALU8080::cmp(regs.A, memory->read(regs.HL()), regs.Flags); asmInstr = "CMP M"; break;
        case 0xBF: ALU8080::cmp(regs.A, regs.A, regs.Flags); asmInstr = "CMP A"; break;

        // --- RNZ ---
        case 0xC0:
            if(!regs.getFlag(Registers8080::ZERO)) {
                regs.PC = popStack();
            }
            asmInstr = "RNZ";
            break;

        // --- ADI d8 ---
        case 0xC6:
            {
                uint8_t data = memory->read(regs.PC++);
                uint8_t flags = regs.Flags;
                regs.A = ALU8080::add(regs.A, data, flags);
                regs.Flags = flags;
                asmInstr = "ADI " + utils.hex8_t[data];
            }
            break;

        // RST 0
        case 0xC7: pushStack(regs.PC); regs.PC = 0x00; asmInstr = "RST 0"; break;
        case 0xCF: pushStack(regs.PC); regs.PC = 0x08; asmInstr = "RST 1"; break;
        case 0xD7: pushStack(regs.PC); regs.PC = 0x10; asmInstr = "RST 2"; break;
        case 0xDF: pushStack(regs.PC); regs.PC = 0x18; asmInstr = "RST 3"; break;
        case 0xE7: pushStack(regs.PC); regs.PC = 0x20; asmInstr = "RST 4"; break;
        case 0xEF: pushStack(regs.PC); regs.PC = 0x28; asmInstr = "RST 5"; break;
        case 0xF7: pushStack(regs.PC); regs.PC = 0x30; asmInstr = "RST 6"; break;
        case 0xFF: pushStack(regs.PC); regs.PC = 0x38; asmInstr = "RST 7"; break;

        // --- RZ ---
        case 0xC8: if(regs.getFlag(Registers8080::ZERO)) regs.PC = popStack(); asmInstr = "RZ"; break;
        case 0xD0: if(!regs.getFlag(Registers8080::CARRY)) regs.PC = popStack(); asmInstr = "RNC"; break;
        case 0xD8: if(regs.getFlag(Registers8080::CARRY)) regs.PC = popStack(); asmInstr = "RC"; break;

        // --- ACI d8 ---
        case 0xCE: {
                uint8_t data = memory->read(regs.PC++);
                uint8_t flags = regs.Flags;
                uint8_t carry = (flags & Registers8080::CARRY) ? 1: 0;
                regs.A = ALU8080::add(regs.A, data + carry, flags);
                regs.Flags = flags;
                asmInstr = "ACI " + utils.hex8_t[data];
            }
            break;

        // --- SUI d8 ---
        case 0xD6: {
                uint8_t data = memory->read(regs.PC++);
                uint8_t flags = regs.Flags;
                regs.A = ALU8080::sub(regs.A, data, flags);
                regs.Flags = flags;
                asmInstr = "SUI " + utils.hex8_t[data];
            }
            break;

        case 0xCB: asmInstr = "NOP"; break;
        case 0xD9: asmInstr = "NOP"; break;
        case 0xDD: asmInstr = "NOP"; break;
        case 0xED: asmInstr = "NOP"; break;

        // --- SBI d8 ---
        case 0xDE:
            {
                uint8_t data = memory->read(regs.PC++);
                uint8_t flags = regs.Flags;
                uint8_t carry = (flags & Registers8080::CARRY) ? 1: 0;
                regs.A = ALU8080::sub(regs.A, data + carry, flags);
                regs.Flags = flags;
                asmInstr = "SBI " + utils.hex8_t[data];
            }
            break;

        // --- RPO ---
        case 0xE0: if(!regs.getFlag(Registers8080::PARITY)) regs.PC = popStack(); asmInstr = "RPO"; break;

        // --- XTHL ---
        case 0xE3:
            {
                uint16_t temp = regs.HL();
                regs.H = memory->read(regs.SP + 1);
                regs.L = memory->read(regs.SP);
                memory->write(regs.SP, (temp >> 8) & 0xFF);
                memory->write(regs.SP, temp & 0xFF);
                asmInstr = "XTHL";
            }
            break;

        // --- RPE ---
        case 0xE8: if(regs.getFlag(Registers8080::PARITY)) regs.PC = popStack(); asmInstr = "RPE"; break;

        // --- PCHL ---
        case 0xE9: regs.PC = regs.HL(); asmInstr = "PCHL"; break;

        // --- XCHG ---
        case 0xEB:
            {
                uint8_t tempH = regs.H;
                uint8_t tempL = regs.L;
                regs.H = regs.D;
                regs.L = regs.E;
                regs.D = tempH;
                regs.E = tempL;
                asmInstr = "XCHG";
            }
            break;

        // --- RP ---
        case 0xF0: if(!regs.getFlag(Registers8080::SIGN)) regs.PC = popStack(); asmInstr = "RP"; break;

        // --- JP addr ---
        case 0xF2:
            {
                uint16_t addr = memory->read(regs.PC++) | (memory->read(regs.PC++) << 8);
                if(!regs.getFlag(Registers8080::SIGN)) regs.PC = addr;
                asmInstr = "JP " + utils.hex16_t[addr];
            }
            break;

        // --- DI ---
        case 0xF3: interruptsEnabled = false; asmInstr = "DI"; break;

        // --- EI ---
        case 0xFB: interruptsEnabled = true; asmInstr = "EI"; break;

        // --- RM ---
        case 0xF8: if(regs.getFlag(Registers8080::SIGN)) regs.PC = popStack(); asmInstr = "RM"; break;

        // --- SPHL ---
        case 0xF9: regs.SP = regs.HL(); asmInstr = "SPHL"; break;

        case 0xFD: asmInstr = "NOP"; break;

        default:
            std::cout << "[Nieznana instrukcja] 0x" << std::hex << (int)opcode << std::endl;
            asmInstr = "??? 0x" + utils.hex8_t[opcode];
            break;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    lastCycleTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    bool notifyCycleTooLong = false, notifyCycleTooShort = false, notifyCycleEqual = false;
    int executeTime = lastCycleTime - cycleDuration;

    if(executeTime > 0) {
        busyWait(std::chrono::microseconds(executeTime));
        notifyCycleTooLong = true;
        currentTimer += executeTime;
    } else if(executeTime < 0) {
        notifyCycleTooShort = true;
        currentTimer += executeTime;
    } else {
        notifyCycleEqual = true;
    }

    if(currentTimer < -10) {
        busyWait(std::chrono::microseconds(-currentTimer));
        totalSpeeding += currentTimer;
        currentTimer = 0;
    }

    addLastInstruction(asmInstr);

    std::cout 
        << "|" << executedInstructions << "| "
        << "[" 
        << utils.hex16_t[regs.PC] 
        << "][ " 
        << std::hex 
        << static_cast<int>(opcode) 
        << " ] " 
        << asmInstr 
        // << " (cycles: " 
        // << std::dec
        // << instructionCycles[opcode] 
        // << ") realtime: " 
        // << lastCycleTime 
        // << "us, expected: " 
        // << cycleDuration 
        // << "us" 
        // << (notifyCycleTooLong ? " \033[31m(CYCLE TOO LONG!)\033[0m" : "")
        // << (notifyCycleTooShort ? " \033[33m(CYCLE TOO SHORT!)\033[0m" : "")
        // << (notifyCycleEqual ? " \033[32m(CYCLE OK)\033[0m" : "")
        // << " TIMER: " << currentTimer << "us"
        << std::endl;
}

bool CPU8080::loadProgram(uint16_t startAddress, std::vector<uint8_t>& program) {
    bool loaded = memory->loadProgram(startAddress, program);
    if(loaded) {
        regs.PC = startAddress;
    }
    return loaded;
}

void CPU8080::consoleDump() {
    std::cout << "CPU: (8080)" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Registers: " << std::endl;
    std::cout << "A:  " << utils.hex8_t[regs.A] << std::endl;
    std::cout << "B:  " << utils.hex8_t[regs.B] << std::endl;
    std::cout << "C:  " << utils.hex8_t[regs.C] << std::endl;
    std::cout << "D:  " << utils.hex8_t[regs.D] << std::endl;
    std::cout << "E:  " << utils.hex8_t[regs.E] << std::endl;
    std::cout << "H:  " << utils.hex8_t[regs.H] << std::endl;
    std::cout << "L:  " << utils.hex8_t[regs.L] << std::endl;
    std::cout << "PC: " << utils.hex16_t[regs.PC] << std::endl;
    std::cout << "SP: " << utils.hex16_t[regs.SP] << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Flags: " << std::endl;
    std::cout << "Z:  " << regs.getFlag(Registers8080::ZERO) << std::endl;
    std::cout << "S:  " << regs.getFlag(Registers8080::SIGN) << std::endl;
    std::cout << "P:  " << regs.getFlag(Registers8080::PARITY) << std::endl;
    std::cout << "CY: " << regs.getFlag(Registers8080::CARRY) << std::endl;
    std::cout << "AC: " << regs.getFlag(Registers8080::AUX_CARRY) << std::endl;
}

void CPU8080::testOpcodes() {
    std::cout << "Testing Intel 8080 CPU opcodes..." << std::endl;
    for(uint16_t opcode=0; opcode<=0xFF; opcode++) {
        regs.reset();
        setRunning(true);
        memory->write(0x1000, opcode);
        memory->write(0x1001, 0x00);
        memory->write(0x1002, 0x00);
        regs.PC = 0x1000;

        std::cout << "TEST> opcode 0x" << std::hex << opcode << " -> ";
        try {
            step();
            std::cout << "OK" << std::endl;
        } catch(const std::exception& e) {
            std::cout << "FAILED: " << e.what() << std::endl;
        }
    }
    std::cout << "Testing opcodes done." << std::endl;
}
