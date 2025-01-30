#pragma once

#include "TerminalBase.h"
#include "CPUZ80.h"
#include "helpers/extensions/toLower.h"

#include <sstream>
#include <string>

class TerminalZ80 : public TerminalBase<TerminalZ80, CPUZ80> {
    public:
        TerminalZ80(CPUZ80& cpu, int widthChars, int heightChars, int widthPixels, int heightPixels)
            : TerminalBase(cpu, widthChars, heightChars, widthPixels, heightPixels) {}

        void processCommand(const std::string& command) {
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

    private:
        void displayMemory(uint16_t startAddress, uint16_t size) {
            constexpr int bytesPerLine = 16;
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
                        printString(" %02X", cpu.getMemory().read(currentAddr));
                    } else {
                        printString("   ");
                    }
                }
                printString("\n");
            }
        }
};
