#include "EmulatorManager.h"

EmulatorManager::EmulatorManager() {
    // --- 8080 create
    auto terminal8080 = std::make_shared<Terminal8080>(80, 25, 800, 600);
    auto disk8080 = std::make_shared<DiskController8080>();
    io8080 = std::make_shared<IO8080>(terminal8080, disk8080);
    memory8080 = std::make_shared<Memory8080>();

    terminal8080->setMemory(memory8080);
    memory8080->setIO(io8080);

    cpu8080 = std::make_shared<CPU8080>(memory8080, io8080);
    cpmbios8080 = std::make_shared<CPMBIOS8080>(cpu8080.get(), memory8080.get(), io8080.get());

    // init 8080 emulator window
    emulatorWindow8080 = std::make_shared<EmulatorWindow8080>(cpu8080);

    // --- Z80 create
    terminalZ80 = std::make_shared<TerminalZ80>(80, 25, 800, 600);
    diskZ80 = std::make_shared<DiskControllerZ80>();
    ioZ80 = std::make_shared<IOZ80>();
    memoryZ80 = std::make_shared<MemoryZ80>();
    // cpmbiosZ80 = std::make_shared<CPMBIOSZ80>(cpuZ80.get(), memoryZ80.get(), terminalZ80.get(), diskZ80.get());
    
    terminalZ80->setMemory(memoryZ80);
    ioZ80->setTerminal(terminalZ80);
    ioZ80->setDisk(diskZ80);

    cpuZ80 = std::make_shared<CPUZ80>(memoryZ80, ioZ80);

    // init Z80 emulator window
    emulatorWindowZ80 = std::make_shared<EmulatorWindowZ80>(cpuZ80);
}
