#include "EmulatorManager.h"

EmulatorManager::EmulatorManager() {
    // --- 8080 create
    terminal8080 = std::make_shared<Terminal8080>(80, 25, 800, 600);
    disk8080 = std::make_shared<DiskController>();
    io8080 = std::make_shared<IO8080>();
    memory8080 = std::make_shared<Memory8080>();

    terminal8080->setMemory(memory8080);
    io8080->setTerminal(terminal8080);
    io8080->setDisk(disk8080);
    memory8080->setIO(io8080);

    cpu8080 = std::make_shared<CPU8080>(terminal8080,memory8080, io8080);
    emulatorWindow8080 = std::make_shared<EmulatorWindow8080>(cpu8080);

    // --- Z80 create
    terminalZ80 = std::make_shared<TerminalZ80>(80, 25, 800, 600);
    diskZ80 = std::make_shared<DiskController>();
    ioZ80 = std::make_shared<IOZ80>();
    memoryZ80 = std::make_shared<MemoryZ80>();

    terminalZ80->setMemory(memoryZ80);
    ioZ80->setTerminal(terminalZ80);
    ioZ80->setDisk(diskZ80);

    cpuZ80 = std::make_shared<CPUZ80>(memoryZ80, ioZ80);
    emulatorWindowZ80 = std::make_shared<EmulatorWindowZ80>(cpuZ80);
}
