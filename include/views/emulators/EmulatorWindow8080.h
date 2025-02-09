#pragma once

#include "CPU8080.h"
#include "EmulatorWindowBase.h"
#include "Terminal8080.h"
#include "CPMBIOS8080.h"

#include <memory>

class EmulatorWindow8080 : public EmulatorWindowBase<EmulatorWindow8080, CPU8080, CPMBIOS8080> {
    public:
        EmulatorWindow8080(std::shared_ptr<CPU8080> cpuPtr, std::shared_ptr<CPMBIOS8080> cpmbiosPtr);
        ~EmulatorWindow8080() = default;

        void start();

    private:
        std::shared_ptr<CPU8080> cpu;
        std::shared_ptr<CPMBIOS8080> cpmbios;

        bool memoryViewVisible = false;
        int memoryViewStartAddress = 0x0000;
        float buttonsY = 16 * 25 + 2 * 20;

        float winWidth, termX, panelWidth, pX;

        void guiRegisterBox(float x, float y, const char* label, uint8_t regValue, bool editable = false);
        void guiRegisterBox(float x, float y, const char* label, uint16_t regValue, bool editable = false);
        void guiFlagBox(float x, float y, const char* label, int flagValue, bool editable = false);

        void renderAndHandleButtons();

    protected:
        void renderTerminal();
        void renderRegistersBox();
        void renderFlagsBox();
        void renderMemoryBox(uint16_t startAddress);
        void renderDisassemblyBox();

        uint16_t startAddressForLoadedProgram;
};
