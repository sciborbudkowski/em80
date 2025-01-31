#pragma once

#include "CPU8080.h"
#include "CPUZ80.h"
#include "Memory8080.h"
#include "MemoryZ80.h"
#include "IO8080.h"
#include "IOZ80.h"
#include "Terminal8080.h"
#include "TerminalZ80.h"
#include "DiskController.h"
#include "EmulatorWindow8080.h"
#include "EmulatorWindowZ80.h"

#include <memory>

class EmulatorManager {
    public:
        EmulatorManager();
        ~EmulatorManager() = default;

        std::shared_ptr<ICPU> getCPU8080() { return cpu8080; }
        std::shared_ptr<ICPU> getCPUZ80() { return cpuZ80; }

        std::shared_ptr<EmulatorWindow8080> getEmulatorWindow8080() { return emulatorWindow8080; }
        std::shared_ptr<EmulatorWindowZ80> getEmulatorWindowZ80() { return emulatorWindowZ80; }
        
    private:
        std::shared_ptr<Terminal8080> terminal8080;
        std::shared_ptr<DiskController> disk8080;
        std::shared_ptr<IO8080> io8080;
        std::shared_ptr<Memory8080> memory8080;

        std::shared_ptr<TerminalZ80> terminalZ80;
        std::shared_ptr<DiskController> diskZ80;
        std::shared_ptr<IOZ80> ioZ80;
        std::shared_ptr<MemoryZ80> memoryZ80;

        std::shared_ptr<CPU8080> cpu8080;
        std::shared_ptr<CPUZ80> cpuZ80;

        std::shared_ptr<EmulatorWindow8080> emulatorWindow8080;
        std::shared_ptr<EmulatorWindowZ80> emulatorWindowZ80;
};
