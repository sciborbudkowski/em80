#include "CPUZ80.h"
#include "RegistersZ80.h"
#include "MemoryZ80.h"
#include "ALUZ80.h"
#include "CPUUtils.h"

void CPUZ80::reset() {
    regs.reset();
    memory->clear();
    running = true;
    interruptsEnabled = false;
    
    // io.getTerminal().clear();
    // const char* welcomeText = "Zilog Z80 Emulator.";
    // io.getTerminal().printString(welcomeText);
    // io.getTerminal().setCursorPos(0, 2);
}

void CPUZ80::step() {
    if (!isRunning()) return;

    uint16_t opcode = memory->read(regs.PC++);
    if(opcode == 0xCB || opcode == 0xDD || opcode == 0xED || opcode == 0xFD) {
        decodeAndExecute(opcode, memory->read(regs.PC++));
    } else {
        decodeAndExecute(opcode);
    }
}

void CPUZ80::decodeAndExecute(uint8_t opcode, uint8_t subOpcode) {
    std::string asmInstr;

    switch(opcode) {
        case 0x00: asmInstr = "NOP"; break;
        case 0x76: setRunning(false); asmInstr = "HALT"; regs.PC++; break;

        // data transfer instructions
        case 0x40: regs.B = regs.B; asmInstr = "LD B, B"; regs.PC++; break;
        case 0x41: regs.B = regs.C; asmInstr = "LD B, C"; regs.PC++; break;
        case 0x42: regs.B = regs.D; asmInstr = "LD B, D"; regs.PC++; break;
        case 0x43: regs.B = regs.E; asmInstr = "LD B, E"; regs.PC++; break;
        case 0x44: regs.B = regs.H; asmInstr = "LD B, H"; regs.PC++; break;
        case 0x45: regs.B = regs.L; asmInstr = "LD B, L"; regs.PC++; break;
        case 0x46: regs.B = memory->read(regs.HL()); asmInstr = "LD B, (HL)"; regs.PC++; break;
        case 0x47: regs.B = regs.A; asmInstr = "LD B, A"; regs.PC++; break;
        case 0x48: regs.C = regs.B; asmInstr = "LD C, B"; regs.PC++; break;
        case 0x49: regs.C = regs.C; asmInstr = "LD C, C"; regs.PC++; break;
        case 0x4A: regs.C = regs.D; asmInstr = "LD C, D"; regs.PC++; break;
        case 0x4B: regs.C = regs.E; asmInstr = "LD C, E"; regs.PC++; break;
        case 0x4C: regs.C = regs.H; asmInstr = "LD C, H"; regs.PC++; break;
        case 0x4D: regs.C = regs.L; asmInstr = "LD C, L"; regs.PC++; break;
        case 0x4E: regs.C = memory->read(regs.HL()); asmInstr = "LD C, (HL)"; regs.PC++; break;
        case 0x4F: regs.C = regs.A; asmInstr = "LD C, A"; regs.PC++; break;
        case 0x50: regs.D = regs.B; asmInstr = "LD D, B"; regs.PC++; break;
        case 0x51: regs.D = regs.C; asmInstr = "LD D, C"; regs.PC++; break;
        case 0x52: regs.D = regs.D; asmInstr = "LD D, D"; regs.PC++; break;
        case 0x53: regs.D = regs.E; asmInstr = "LD D, E"; regs.PC++; break;
        case 0x54: regs.D = regs.H; asmInstr = "LD D, H"; regs.PC++; break;
        case 0x55: regs.D = regs.L; asmInstr = "LD D, L"; regs.PC++; break;
        case 0x56: regs.D = memory->read(regs.HL()); asmInstr = "LD D, (HL)"; regs.PC++; break;
        case 0x57: regs.D = regs.A; asmInstr = "LD D, A"; regs.PC++; break;
        case 0x58: regs.E = regs.B; asmInstr = "LD E, B"; regs.PC++; break;
        case 0x59: regs.E = regs.C; asmInstr = "LD E, C"; regs.PC++; break;
        case 0x5A: regs.E = regs.D; asmInstr = "LD E, D"; regs.PC++; break;
        case 0x5B: regs.E = regs.E; asmInstr = "LD E, E"; regs.PC++; break;
        case 0x5C: regs.E = regs.H; asmInstr = "LD E, H"; regs.PC++; break;
        case 0x5D: regs.E = regs.L; asmInstr = "LD E, L"; regs.PC++; break;
        case 0x5E: regs.E = memory->read(regs.HL()); asmInstr = "LD E, (HL)"; regs.PC++; break;
        case 0x5F: regs.E = regs.A; asmInstr = "LD E, A"; regs.PC++; break;
        case 0x60: regs.H = regs.B; asmInstr = "LD H, B"; regs.PC++; break;
        case 0x61: regs.H = regs.C; asmInstr = "LD H, C"; regs.PC++; break;
        case 0x62: regs.H = regs.D; asmInstr = "LD H, D"; regs.PC++; break;
        case 0x63: regs.H = regs.E; asmInstr = "LD H, E"; regs.PC++; break;
        case 0x64: regs.H = regs.H; asmInstr = "LD H, H"; regs.PC++; break;
        case 0x65: regs.H = regs.L; asmInstr = "LD H, L"; regs.PC++; break;
        case 0x66: regs.H = memory->read(regs.HL()); asmInstr = "LD H, (HL)"; regs.PC++; break;
        case 0x67: regs.H = regs.A; asmInstr = "LD H, A"; regs.PC++; break;
        case 0x68: regs.L = regs.B; asmInstr = "LD L, B"; regs.PC++; break;
        case 0x69: regs.L = regs.C; asmInstr = "LD L, C"; regs.PC++; break;
        case 0x6A: regs.L = regs.D; asmInstr = "LD L, D"; regs.PC++; break;
        case 0x6B: regs.L = regs.E; asmInstr = "LD L, E"; regs.PC++; break;
        case 0x6C: regs.L = regs.H; asmInstr = "LD L, H"; regs.PC++; break;
        case 0x6D: regs.L = regs.L; asmInstr = "LD L, L"; regs.PC++; break;
        case 0x6E: regs.L = memory->read(regs.HL()); asmInstr = "LD L, (HL)"; regs.PC++; break;
        case 0x6F: regs.L = regs.A; asmInstr = "LD L, A"; regs.PC++; break;
        case 0x70: memory->write(regs.HL(), regs.B); asmInstr = "LD (HL), B"; regs.PC++; break;
        case 0x71: memory->write(regs.HL(), regs.C); asmInstr = "LD (HL), C"; regs.PC++; break;
        case 0x72: memory->write(regs.HL(), regs.D); asmInstr = "LD (HL), D"; regs.PC++; break;
        case 0x73: memory->write(regs.HL(), regs.E); asmInstr = "LD (HL), E"; regs.PC++; break;
        case 0x74: memory->write(regs.HL(), regs.H); asmInstr = "LD (HL), H"; regs.PC++; break;
        case 0x75: memory->write(regs.HL(), regs.L); asmInstr = "LD (HL), L"; regs.PC++; break;
        case 0x77: memory->write(regs.HL(), regs.A); asmInstr = "LD (HL), A"; regs.PC++; break;
        case 0x78: regs.A = regs.B; asmInstr = "LD A, B"; regs.PC++; break;
        case 0x79: regs.A = regs.C; asmInstr = "LD A, C"; regs.PC++; break;
        case 0x7A: regs.A = regs.D; asmInstr = "LD A, D"; regs.PC++; break;
        case 0x7B: regs.A = regs.E; asmInstr = "LD A, E"; regs.PC++; break;
        case 0x7C: regs.A = regs.H; asmInstr = "LD A, H"; regs.PC++; break;
        case 0x7D: regs.A = regs.L; asmInstr = "LD A, L"; regs.PC++; break;
        case 0x7E: regs.A = memory->read(regs.HL()); asmInstr = "LD A, (HL)"; regs.PC++; break;
        case 0x7F: regs.A = regs.A; asmInstr = "LD A, A"; regs.PC++; break;
        case 0x06: regs.B = memory->read(regs.PC++); asmInstr = "LD B, #" + utils.hex8_t[regs.B]; regs.PC++; break;
        case 0x0E: regs.C = memory->read(regs.PC++); asmInstr = "LD C, #" + utils.hex8_t[regs.C]; regs.PC++; break;
        case 0x16: regs.D = memory->read(regs.PC++); asmInstr = "LD D, #" + utils.hex8_t[regs.D]; regs.PC++; break;
        case 0x1E: regs.E = memory->read(regs.PC++); asmInstr = "LD E, #" + utils.hex8_t[regs.E]; regs.PC++; break;
        case 0x26: regs.H = memory->read(regs.PC++); asmInstr = "LD H, #" + utils.hex8_t[regs.H]; regs.PC++; break;
        case 0x2E: regs.L = memory->read(regs.PC++); asmInstr = "LD L, #" + utils.hex8_t[regs.L]; regs.PC++; break;
        case 0x3E: regs.A = memory->read(regs.PC++); asmInstr = "LD A, #" + utils.hex8_t[regs.A]; regs.PC++; break;
        case 0x0A: regs.A = memory->read(regs.BC()); asmInstr = "LD A, (BC)"; regs.PC++; break;
        case 0x1A: regs.A = memory->read(regs.DE()); asmInstr = "LD A, (DE)"; regs.PC++; break;
        case 0x3A: {
            uint16_t addr = ALUZ80::calculateAddress(memory->read(regs.PC++), memory->read(regs.PC++));
            regs.A = memory->read(addr);
            asmInstr = "LD A, (" + utils.hex16_t[addr] + ")";
            regs.PC++;
            break;
        }
        case 0x02: {
            memory->write(regs.BC(), regs.A);
            asmInstr = "LD (BC), A";
            regs.PC++;
            break;
        }
        case 0x12: {
            memory->write(regs.DE(), regs.A);
            asmInstr = "LD (DE), A";
            regs.PC++;
            break;
        }
        case 0x32: {
            uint16_t addr = ALUZ80::calculateAddress(memory->read(regs.PC++), memory->read(regs.PC++));
            memory->write(addr, regs.A);
            asmInstr = "LD (" + utils.hex16_t[addr] + "), A";
            regs.PC++;
            break;
        }
        case 0x22: {
            uint16_t addr = ALUZ80::calculateAddress(memory->read(regs.PC++), memory->read(regs.PC++));
            memory->write(addr, regs.HL());
            asmInstr = "LD (" + utils.hex16_t[addr] + "), HL";
            regs.PC++;
            break;
        }
        case 0x2A: {
            uint16_t addr = regs.HL();
            regs.HL(memory->read(addr));
            asmInstr = "LD HL, (" + utils.hex16_t[addr] + ")";
            regs.PC++;
            break;
        }
        case 0xF9: {
            regs.SP = regs.HL();
            asmInstr = "LD SP, HL";
            regs.PC++;
            break;
        }
        case 0xC5: pushStack(regs.BC()); asmInstr = "PUSH BC"; regs.PC++; break;
        case 0xD5: pushStack(regs.DE()); asmInstr = "PUSH DE"; regs.PC++; break;
        case 0xE5: pushStack(regs.HL()); asmInstr = "PUSH HL"; regs.PC++; break;
        case 0xF5: pushStack(regs.AF()); asmInstr = "PUSH AF"; regs.PC++; break;
        case 0xC1: regs.BC(popStack()); asmInstr = "POP BC"; regs.PC++; break;
        case 0xD1: regs.DE(popStack()); asmInstr = "POP DE"; regs.PC++; break;
        case 0xE1: regs.HL(popStack()); asmInstr = "POP HL"; regs.PC++; break;
        case 0xF1: regs.AF(popStack()); asmInstr = "POP AF"; regs.PC++; break;
        case 0xEB: regs.HL(regs.DE()); asmInstr = "EX DE, HL"; regs.PC++; break;
        case 0x08: ALUZ80::ex_ax_ax(regs); asmInstr = "EX AF, AF'"; regs.PC++; break;
        case 0xE3: {
            uint16_t temp = regs.SP;
            regs.SP = regs.HL();
            regs.HL(temp);
            asmInstr = "EX (SP), HL";
            regs.PC++;
            break;
        }

        case 0x36: {
            uint16_t addr = regs.HL();
            uint8_t value = memory->read(regs.PC++);
            memory->write(addr, value);
            asmInstr = "LD (HL), #" + utils.hex8_t[value];
            break;
        }
    }
}

void CPUZ80::run() {
    while (isRunning()) {
        step();
    }
}

void CPUZ80::pushStack(uint16_t value) {
    memory->write(--regs.SP, (value >> 8) & 0xFF);
    memory->write(--regs.SP, value & 0xFF);
}

uint16_t CPUZ80::popStack() {
    uint16_t value = memory->read(regs.SP) << 8 | memory->read(regs.SP + 1);
    regs.SP += 2;
    return value;
}

bool CPUZ80::loadProgram(uint16_t startAddress, const std::vector<uint8_t>& program)
{
    bool loaded = memory->loadProgram(startAddress, program);
    if(loaded) {
        regs.PC = startAddress;
    }
    return loaded;
}

void CPUZ80::consoleDump()
{
std::cout << "CPU: (8080)" << std::endl;
std::cout << "----------------------------------------" << std::endl;
std::cout << "Registers: (Z80)" << std::endl;
std::cout << "A:   " << utils.hex8_t[regs.A] << std::endl;
std::cout << "B:   " << utils.hex8_t[regs.B] << std::endl;
std::cout << "C:   " << utils.hex8_t[regs.C] << std::endl;
std::cout << "D:   " << utils.hex8_t[regs.D] << std::endl;
std::cout << "E:   " << utils.hex8_t[regs.E] << std::endl;
std::cout << "H:   " << utils.hex8_t[regs.H] << std::endl;
std::cout << "L:   " << utils.hex8_t[regs.L] << std::endl;
std::cout << "PC:  " << utils.hex16_t[regs.PC] << std::endl;
std::cout << "SP:  " << utils.hex16_t[regs.SP] << std::endl;
std::cout << "----------------------------------------" << std::endl;
std::cout << "Flags: " << std::endl;
std::cout << "C:   " << regs.getFlag(RegistersZ80::CARRY) << std::endl;
std::cout << "SUB: " << regs.getFlag(RegistersZ80::SUBTRACT) << std::endl;
std::cout << "P/O: " << regs.getFlag(RegistersZ80::PARITY) << std::endl;
std::cout << "HC:  " << regs.getFlag(RegistersZ80::HALF_CARRY) << std::endl;
std::cout << "Z:   " << regs.getFlag(RegistersZ80::ZERO) << std::endl;
std::cout << "S:   " << regs.getFlag(RegistersZ80::SIGN) << std::endl;
}

void CPUZ80::testOpcodes()
{
    int failed = 0;

    std::cout << "Testing Zilog Z80 opcodes..." << std::endl;
    std::cout << "BASE: " << std::endl;
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
            failed++;
        }
    }
    std::cout << "Testing BASE opcodes done. Passed: " << (0xFF - failed) << " | Failed: " << failed << std::endl;
}
