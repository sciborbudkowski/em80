#include "CPU8080.h"
#include "EmulatorWindowBase.h"

class EmulatorWindow8080 : public EmulatorWindowBase<EmulatorWindow8080, CPU8080> {
    public:
        EmulatorWindow8080(CPU8080& cpu) : EmulatorWindowBase<EmulatorWindow8080, CPU8080>(cpu) {}
        ~EmulatorWindow8080();

        void start();
        void update();

    protected:
        void renderTerminal();
        void renderRegistersBox();
        void renderFlagsBox();
        void renderMemoryBox();
        void renderDisassemblyBox();
};
