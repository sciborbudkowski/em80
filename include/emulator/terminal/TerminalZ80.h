#pragma once

#include "TerminalBase.h"
#include "MemoryZ80.h"
#include "helpers/extensions/toLower.h"

#include <sstream>
#include <string>
#include <memory>

class TerminalZ80 : public TerminalBase<TerminalZ80> {
    public:
        TerminalZ80(int widthChars = 80, int heightChars = 25, int widthPixels = 800, int heightPixels = 600);

        void processCommand(const std::string& command);

        void setMemory(std::shared_ptr<MemoryZ80> memPtr);

    private:
        std::shared_ptr<MemoryZ80> memory;
        const int bytesPerLine = 16;

        void displayMemory(uint16_t startAddress, uint16_t size);
};
