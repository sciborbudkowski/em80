#include "IO8080.h"
#include "Terminal8080.h"
#include "DiskController.h"

IO8080::IO8080()
    : terminal(nullptr), disk(nullptr), diskEnabled(false) {}

void IO8080::setTerminal(std::shared_ptr<Terminal8080> termPtr) { terminal = termPtr; }
void IO8080::setDisk(std::shared_ptr<DiskController> diskPtr) { disk = diskPtr; }

uint8_t IO8080::in(uint16_t port) const {
    if(port >= ports.size()) return 0xFF;

    switch(port) {
        case 0x02: return terminal->getLastChar(); break;
        case 0x10: return diskEnabled ? 0x00 : 0xFF; break;
        case 0x13: return diskEnabled ? disk->read() : 0xFF; break;
    }

    return ports[port];
}

void IO8080::out(uint16_t port, uint8_t value) {
    if(port >= ports.size()) return;

    switch(port) {
        case 0x01: terminal->printChar(value); break;
        case 0x11: /* TODO: drive selection*/ break;
        case 0x12: disk->setSector(value); break;
    }

    ports[port] = value;
}