#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <cstdarg>
#include <sstream>
#include <iostream>
#include <termios.h>

class TerminalBase {
    public:
        TerminalBase(int width = 80, int height = 25) : width(width), height(height) {
            buffer.resize(height, std::string(width, ' '));
        }

        void printChar(uint8_t ch) {
            if(ch == '\n') {
                cursorX = 0;
                cursorY++;
            } else {
                buffer[cursorY][cursorX] = ch;
                cursorX++;

                if(cursorX >= width) {
                    cursorX = 0;
                    cursorY++;
                }
            }

            if(cursorY >= height) {
                buffer.erase(buffer.begin());
                buffer.push_back(std::string(width, ' '));
                cursorY = height - 1;
            }

            lastCommand += ch;
            lastChar = ch;
        }

        void printString(const char *format, ...) {
            constexpr size_t bufferSize = 1024;
            char buffer[bufferSize];

            va_list args;
            va_start(args, format);
            vsnprintf(buffer, bufferSize, format, args);
            va_end(args);

            std::string str(buffer);
            for(char ch : str) {
                printChar(ch);
            }
        }

        void clear() {
            buffer.clear();
            buffer.resize(height, std::string(width, ' '));
            cursorX = 0;
            cursorY = 0;
        }

        void setCursorPos(int newX, int newY) {
            cursorX = newX;
            cursorY = newY;
        }

        void removeLastChar() {
            if(cursorX > 0) {
                cursorX--;
                buffer[cursorY][cursorX] = ' ';
            }
        }

        const std::vector<std::string>& getBuffer() const { return buffer; }

        std::pair<int, int> getCursorPos() const { return {cursorX, cursorY}; }

        void processCommand(const std::string& command) {
            std::istringstream iss(command);
            std::string cmd;
            uint16_t startAddress, size;

            iss >> cmd >> std::hex >> startAddress >> size;

            // if(cmd == "m") {
            //     displayMemory(cpu.memory.ram, startAddress, size);
            // } else {
            //     printString("\nUnknown command: %s", cmd.c_str());
            // }
        }

        std::string getLastCommand() { return lastCommand; }

        void setLastCommand(const std::string& command) {
            if(!command.empty()) {
                lastCommand += command;
            }
        }

        uint8_t getChar() { return lastChar; }

    private:
        int width, height;
        int cursorX, cursorY;
        std::vector<std::string> buffer;
        std::string lastCommand;
        uint8_t lastChar;

        void displayMemory(const std::vector<uint8_t>& memory, uint16_t startAddress, uint16_t size) {
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
                        printString(" %02X", memory[currentAddr]);
                    } else {
                        printString("   ");
                    }
                }
                printString("\n");
            }
        }
};
