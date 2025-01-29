#include "CPUZ80.h"
#include "EmulatorWindowBase.h"

class EmulatorWindowZ80 : public EmulatorWindowBase<EmulatorWindowZ80, CPUZ80> {
    public:
        EmulatorWindowZ80(CPUZ80& cpu) : EmulatorWindowBase<EmulatorWindowZ80, CPUZ80>(cpu) {}

        void update() { cpu.step(); }

        void render() {
            // TODO: Render the emulator window
        }
};
