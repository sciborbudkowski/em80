#include "TerminalZ80.h"

TerminalZ80::TerminalZ80(int widthChars, int heightChars, int widthPixels, int heightPixels)
    : TerminalBase(widthChars, heightChars, widthPixels, heightPixels) {}

void TerminalZ80::processCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    uint16_t startAddress = 0, size = 0;

    if(!(iss >> cmd)) {
        printString("Unknown command.");
        return;
    }

    if(toLower(cmd) == "m") {
        if(!(iss >> std::hex >> startAddress >> size)) {
            printString("Syntax error: m <start_address> <size>");
            return;
        }

        displayMemory(startAddress, size);
    } else {
        printString("Unknown command.");
    }
}

void TerminalZ80::setMemory(std::shared_ptr<MemoryZ80> memPtr) { memory = memPtr; }

void TerminalZ80::displayMemory(uint16_t startAddress, uint16_t size) {
    uint16_t endAddress = startAddress + size;

    printString("Address");
    for(int i=0; i<bytesPerLine; i++) {
        printString(" %02X", startAddress + i);
    }
    printString("\n");

    for(uint16_t addr=startAddress; addr<endAddress; addr+=bytesPerLine) {
        printString("%04Xh", addr);
        for(int i=0; i<bytesPerLine; i++) {
            uint16_t currentAddr = addr + i;
            if(currentAddr < endAddress) {
                printString(" %02X", memory->read(currentAddr));
            } else {
                printString("   ");
            }
        }
        printString("\n");
    }
}