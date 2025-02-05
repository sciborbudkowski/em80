#pragma once

#include "AssetsLoader.h"
#include "MemoryBase.h"

#include <raylib.h>
#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include <sstream>

class TerminalBase {
    public:
        TerminalBase(int widthChars, int heightChars, int widthPixels, int heightPixels)
            : widthChars(widthChars), heightChars(heightChars), widthPixels(widthPixels), heightPixels(heightPixels),
              cursorX(0), cursorY(0) {
            buffer.resize(heightChars, std::string(widthChars, ' '));
        }

        ~TerminalBase() = default;

        void virtual handleInput(const std::string& command, MemoryBase& memory) = 0;

        void initialize() {
            font = AssetsLoader::loadFont("assets/fonts/DejaVuSansMono.ttf");
            updateScaling();
        }

        void updateScaling() {
            fontSize = widthPixels / static_cast<float>(widthChars);
            cellWidth = fontSize;
            cellHeight = heightPixels / static_cast<float>(heightChars);
        }

        void render() {
            for(int y=0; y<heightChars; y++) {
                for(int x=0; x<widthChars; x++) {
                    char ch = buffer[y][x];
                    if(ch != ' ') {
                        Vector2 position = {x*cellWidth, y*cellHeight};
                        DrawTextEx(font, std::string(1, ch).c_str(), position, fontSize, 2, WHITE);
                    }
                }
            }
        }

        void printChar(uint8_t ch) {
            status = false;
            if(ch == '\n') {
                cursorX = 0;
                cursorY++;
                lastCommand.clear();
            } else {
                buffer[cursorY][cursorX] = ch;
                cursorX++;

                if(cursorX >= widthChars) {
                    cursorX = 0;
                    cursorY++;
                }
            }

            if(cursorY >= heightChars) {
                buffer.erase(buffer.begin());
                buffer.push_back(std::string(widthChars, ' '));
                cursorY = heightChars - 1;
            }
            status = true;
        }

        void printNewLine() {
            cursorX = 0;
            cursorY++;
        }

        std::string getLastCommand() const { return lastCommand; }

        void setLastCommand(const std::string& command) { lastCommand = command; }

        std::vector<std::string>& getBuffer() { return buffer; }

        void printString(const char* format, ...) {
            status = false;
            va_list args;
            va_start(args, format);

            int size = std::vsnprintf(nullptr, 0, format, args);
            va_end(args);

            if(size < 0) return;

            std::vector<char> buffer(size);
            va_start(args, format);
            std::vsnprintf(buffer.data(), size, format, args);
            va_end(args);

            for(char ch : buffer) {
                if(ch == '\0') break;
                printChar(ch);
            }
            status = true;
        }

        void clear() {
            status = false;
            buffer.clear();
            buffer.resize(heightChars, std::string(widthChars, ' '));
            cursorX = 0;
            cursorY = 0;
            status = true;
        }

        void setCursorPos(int newX, int newY) {
            status = false;
            cursorX = newX;
            cursorY = newY;
            status = true;
        }

        void removeLastChar() {
            status = false;
            if(cursorX > 0) {
                cursorX--;
                buffer[cursorY][cursorX] = ' ';
            } else if(cursorY > 0) {
                cursorY--;
                cursorX = widthChars - 1;
                buffer[cursorY][cursorX] = ' ';
            }
            status = true;
        }

        std::pair<int, int> getCursorPos() const { return {cursorX, cursorY}; }

        char getLastChar() const { return lastChar; }

        bool getStatus() const { return status; }

    protected:
        int widthChars, heightChars;
        int widthPixels, heightPixels;
        float fontSize;
        float cellWidth, cellHeight;
        int cursorX, cursorY;
        char lastChar;
        bool status = true;

        Font font;
        std::vector<std::string> buffer;
        std::string lastCommand;
};


// #pragma once

// #include "AssetsLoader.h"

// #include <raylib.h>
// #include <string>
// #include <vector>
// #include <cstdint>
// #include <iostream>
// #include <sstream>

// template <typename Derived>
// class TerminalBase {
//     public:
//         TerminalBase(int widthChars, int heightChars, int widthPixels, int heightPixels)
//             : widthChars(widthChars), heightChars(heightChars), widthPixels(widthPixels), heightPixels(heightPixels),
//               cursorX(0), cursorY(0) {
//             buffer.resize(heightChars, std::string(widthChars, ' '));
//         }

//         ~TerminalBase() = default;

//         void handleInput(const std::string& command) { static_cast<Derived*>(this)->handleInput(command); }

//         void initialize() {
//             font = AssetsLoader::loadFont("assets/fonts/DejaVuSansMono.ttf");
//             updateScaling();
//         }

//         void updateScaling() {
//             fontSize = widthPixels / static_cast<float>(widthChars);
//             cellWidth = fontSize;
//             cellHeight = heightPixels / static_cast<float>(heightChars);
//         }

//         void render() {
//             for(int y=0; y<heightChars; y++) {
//                 for(int x=0; x<widthChars; x++) {
//                     char ch = buffer[y][x];
//                     if(ch != ' ') {
//                         Vector2 position = {x*cellWidth, y*cellHeight};
//                         DrawTextEx(font, std::string(1, ch).c_str(), position, fontSize, 2, WHITE);
//                     }
//                 }
//             }
//         }

//         void printChar(uint8_t ch) {
//             status = false;
//             if(ch == '\n') {
//                 cursorX = 0;
//                 cursorY++;
//                 lastCommand.clear();
//             } else {
//                 buffer[cursorY][cursorX] = ch;
//                 cursorX++;

//                 if(cursorX >= widthChars) {
//                     cursorX = 0;
//                     cursorY++;
//                 }
//             }

//             if(cursorY >= heightChars) {
//                 buffer.erase(buffer.begin());
//                 buffer.push_back(std::string(widthChars, ' '));
//                 cursorY = heightChars - 1;
//             }
//             status = true;
//         }

//         void printNewLine() {
//             cursorX = 0;
//             cursorY++;
//         }

//         std::string getLastCommand() const { return lastCommand; }

//         void setLastCommand(const std::string& command) { lastCommand = command; }

//         std::vector<std::string>& getBuffer() { return buffer; }

//         void printString(const char* format, ...) {
//             status = false;
//             va_list args;
//             va_start(args, format);

//             int size = std::vsnprintf(nullptr, 0, format, args);
//             va_end(args);

//             if(size < 0) return;

//             std::vector<char> buffer(size);
//             va_start(args, format);
//             std::vsnprintf(buffer.data(), size, format, args);
//             va_end(args);

//             for(char ch : buffer) {
//                 if(ch == '\0') break;
//                 printChar(ch);
//             }
//             status = true;
//         }

//         void clear() {
//             status = false;
//             buffer.clear();
//             buffer.resize(heightChars, std::string(widthChars, ' '));
//             cursorX = 0;
//             cursorY = 0;
//             status = true;
//         }

//         void setCursorPos(int newX, int newY) {
//             status = false;
//             cursorX = newX;
//             cursorY = newY;
//             status = true;
//         }

//         void removeLastChar() {
//             status = false;
//             if(cursorX > 0) {
//                 cursorX--;
//                 buffer[cursorY][cursorX] = ' ';
//             } else if(cursorY > 0) {
//                 cursorY--;
//                 cursorX = widthChars - 1;
//                 buffer[cursorY][cursorX] = ' ';
//             }
//             status = true;
//         }

//         std::pair<int, int> getCursorPos() const { return {cursorX, cursorY}; }

//         char getLastChar() const { return lastChar; }

//         bool getStatus() const { return status; }

//     protected:
//         int widthChars, heightChars;
//         int widthPixels, heightPixels;
//         float fontSize;
//         float cellWidth, cellHeight;
//         int cursorX, cursorY;
//         char lastChar;
//         bool status = true;

//         Font font;
//         std::vector<std::string> buffer;
//         std::string lastCommand;
// };
