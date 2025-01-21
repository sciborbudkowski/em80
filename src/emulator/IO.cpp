/*
#include "IO.h"
#include "Terminal.h"
#include "Registers.h"
#include "Disk.h"

#include <QMessageBox>
#include <QString>
#include <iostream>
#include <chrono>

extern Disk disk;
extern Terminal terminal;
extern Registers regs;

uint8_t IO::input(uint8_t port) {
    switch(port) {
        // --- keyboard/terminal ---
        case 0x00: {
            if(keyBuffer) {
                uint8_t key = keyBuffer;
                keyBuffer = 0;
                return key;
            }
            return 0;
        }

        // --- controls ---
        case 0x10: return 0;

        // --- timer ---
        case 0x20:
            return static_cast<uint8_t>(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now().time_since_epoch()
            ).count() & 0xFF);

        // --- disk ---
        case 0x30: return disk->read(regs->getRegisterPair(_HL));
        case 0x32:
            return 0;

        default:
            std::cout << "Unknown port: " << std::hex << port << std::endl;
            return 0;
    }
}

void IO::output(uint8_t port, uint8_t value) {
    switch(port) {
        case 0x01:
            term.printChar(value);
            break;

        case 0x11: break;

        case 0x21:
            std::cout << "[Timer IRQ] IRQ sent from port 0x21" << std::endl;
            break;

        case 0x31:
            disk->write(regs->getRegisterPair(_HL), value);
            break;

        case 0x33:
            disk->saveDiskToFile("disk.img");
            break;

        default:
            std::cout << "Unknown port: " << std::hex << port << std::endl;
    }
}

void IO::registerKeyPress(char key) {
    keyBuffer = key;
}

void IO::writeToTerminal(uint8_t ch) {
    term.printChar(ch);
}

void IO::clearTerminal() {
    term.clear();
}

Terminal& IO::getTerminal() {
    return term;
}
*/