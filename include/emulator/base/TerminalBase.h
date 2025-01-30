#pragma once

#include <raylib.h>
#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include <sstream>

template <typename Derived, typename CPU>
class TerminalBase {
    public:
        TerminalBase(CPU& cpu, int widthChars, int heightChars, int widthPixels, int heightPixels)
            : cpu(cpu), widthChars(widthChars), heightChars(heightChars), widthPixels(widthPixels), heightPixels(heightPixels),
              cursorX(0), cursorY(0) {
            buffer.resize(heightChars, std::string(widthChars, ' '));
        }

        ~TerminalBase() { if(font) UnloadFont(font); }

        void handleInput(const std::string& command) { static_cast<Derived*>(this)->handleInput(command); }

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

                lastCommand += ch;
            }

            if(cursorY >= heightChars) {
                buffer.erase(buffer.begin());
                buffer.push_back(std::string(widthChars, ' '));
                cursorY = heightChars - 1;
            }
        }

        std::string getLastCommand() const { return lastCommand; }
        void setLastCommand(const std::string& command) { lastCommand = command; }

        std::vector<std::string>& getBuffer() { return buffer; }

        void printString(const char* format, ...) {
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
        }

        void clear() {
            buffer.clear();
            buffer.resize(heightChars, std::string(widthChars, ' '));
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

        void removeLastChar() {
            if(cursorX > 0) {
                cursorX--;
                buffer[cursorY][cursorX] = ' ';
            } else if(cursorY > 0) {
                cursorY--;
                cursorX = widthChars - 1;
                buffer[cursorY][cursorX] = ' ';
            }
        }

        std::pair<int, int> getCursorPos() const { return {cursorX, cursorY}; }

        char getLastChar() const { return lastChar; }

    protected:
        CPU& cpu;

        int widthChars, heightChars;
        int widthPixels, heightPixels;
        float fontSize;
        float cellWidth, cellHeight;
        int cursorX, cursorY;
        char lastChar;

        Font font;
        std::vector<std::string> buffer;
        std::string lastCommand;
};
