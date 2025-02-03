#pragma once

// #define RAYGUI_IMPLEMENTATION
#include "raylib_wrapper.h"
#include "raygui.h"

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 490

template <typename Derived, typename CPUType, typename CPMBiosOptionalType = void>
class EmulatorWindowBase {
   public:
        EmulatorWindowBase(CPUType& cpu) : cpu(cpu), cpmBIOS(nullptr) {}
        EmulatorWindowBase(CPUType& cpu, CPMBiosOptionalType*bios) : cpu(cpu), cpmBIOS(bios) {}

        void start() { static_cast<Derived*>(this)->start(); }
        void update() { static_cast<Derived*>(this)->update(); }

    protected:
        CPUType& cpu;
        CPMBiosOptionalType* cpmBIOS;

        void renderTerminal() { static_cast<Derived*>(this)->renderTerminal(); }
        void renderRegistersBox() { static_cast<Derived*>(this)->renderRegistersBox(); }
        void renderFlagsBox() { static_cast<Derived*>(this)->renderFlagsBox(); }
        void renderMemoryBox() { static_cast<Derived*>(this)->renderMemoryBox(); }
        void renderDisassemblyBox() { static_cast<Derived*>(this)->renderDisassemblyBox(); }

        Font font;
};
