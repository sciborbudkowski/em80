#include "CPUZ80.h"
#include "EmulatorWindowBase.h"
#include "TerminalZ80.h"

#include <memory>

class EmulatorWindowZ80 : public EmulatorWindowBase<EmulatorWindowZ80, CPUZ80> {
    public:
        EmulatorWindowZ80(std::shared_ptr<CPUZ80> cpu)
            : EmulatorWindowBase<EmulatorWindowZ80, CPUZ80>(*cpu) {}
        ~EmulatorWindowZ80() = default;

        void start() { /* empty for now */ };
        void update() { /* empty for now */ };

    private:
        std::shared_ptr<CPUZ80> cpu;

        void renderTerminal() { /* empty for now */ };
        void renderRegistersBox() { /* empty for now */ };
        void renderFlagsBox() { /* empty for now */ };
        void renderMemoryBox(uint16_t startAddress) { /* empty for now */ };
        void renderDisassemblyBox() { /* empty for now */ };
};
