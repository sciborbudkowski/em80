#pragma once

#include "TerminalBase.h"
#include "helpers/extensions/toLower.h"

#include <sstream>
#include <string>
#include <memory>

struct Memory8080;

class Terminal8080 : public TerminalBase<Terminal8080> {
    public:
        Terminal8080(int widthChars = 80, int heightChars = 25, int widthPixels = 800, int heightPixels = 600);

        void handleInput(const std::string& command);

        void setMemory(std::shared_ptr<Memory8080> memPtr);

    private:
        std::shared_ptr<Memory8080> memory;

        void displayMemory(uint16_t startAddress, uint16_t size);
};
