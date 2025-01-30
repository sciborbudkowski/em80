#include "CPU8080.h"
#include "EmulatorWindowBase.h"
#include "Terminal8080.h"

class EmulatorWindow8080 : public EmulatorWindowBase<EmulatorWindow8080, CPU8080> {
    public:
        EmulatorWindow8080(CPU8080& cpu)
            : EmulatorWindowBase<EmulatorWindow8080, CPU8080>(cpu) {}
        ~EmulatorWindow8080();

        void start();
        void update();

    private:
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
};
