#pragma once

#include "TerminalBase.h"
#include "helpers/extensions/toLower.h"

#include <sstream>
#include <string>
#include <memory>

struct Memory8080;

class Terminal8080 : public TerminalBase {
    public:
        Terminal8080(int widthChars = 80, int heightChars = 25, int widthPixels = 800, int heightPixels = 600);

        void handleInput(const std::string& command, MemoryBase& memory) override {
            std::istringstream iss(command);
            std::string cmd;
            uint16_t startAddress = 0, size = 0;

            if(command.empty()) return;

            if(!(iss >> cmd)) {
                printString("Unknown command.");
                return;
            }

            if(toLower(cmd) == "m") {
                if(!(iss >> std::hex >> startAddress >> size)) {
                    printString("Syntax error: m <start_address> <size>");
                    return;
                }

                // memory dump to terminal
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
                            printString(" %02X", memory.read(currentAddr));
                        } else {
                            printString("   ");
                        }
                    }
                    printString("\n");
                }
            } else {
                printString("Unknown command.");
            }
        }
};
