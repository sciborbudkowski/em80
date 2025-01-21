#pragma once

#include "CPUBase.h"
#include "CPUType.h"
#include "CPUFactory.h"
#include "MemoryBase.h"
#include "TerminalBase.h"

#include "raylib_wrapper.h"

class EmulatorWindow {
    public:
        EmulatorWindow(CPUType type);
        ~EmulatorWindow() = default;

        void start();

    private:
        void drawTerminal(const TerminalBase& terminal);
        void GuiRegisterBox(int x, int y, const char* label, Register8& reg, bool editable = false);
        void GuiRegisterBox(int x, int y, const char* label, Register16& reg, bool editable = false);
        void GuiRegisterBox(int x, int y, const char* label, bool& reg, bool editable = false);
        void drawMemoryView(MemoryBase& memory, int startAddress);

        std::unique_ptr<CPUBase> cpu;
        bool running = false;
        bool memoryViewVisible = false;
        int memoryViewStartAddress = 0x0000;
        float buttonsY = 16 * 25 + 2 * 20;
        Font font;
};
