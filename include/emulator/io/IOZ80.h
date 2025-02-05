#pragma once

#include "IOBase.h"
#include "TerminalZ80.h"
#include "DiskControllerZ80.h"

#include <cstdint>
#include <vector>
#include <memory>

struct IOZ80 : public IOBase<TerminalZ80, DiskControllerZ80> {
    std::vector<uint8_t> ports = std::vector<uint8_t>(0xFF, 0xFF);
    bool diskEnabled = false;

    IOZ80(std::shared_ptr<TerminalZ80> terminal, std::shared_ptr<DiskControllerZ80> disk) : IOBase(std::make_unique<TerminalZ80>(terminal), std::make_unique<DiskControllerZ80>(disk)) {}

    uint8_t in(uint8_t port) const {
        if(port >= ports.size()) return 0xFF;

        switch(port) {
            case 0x02: return '\0'; break; // return terminal->getLastChar(); break;
            case 0x10: return diskEnabled ? 0x00 : 0xFF; break;
            case 0x13: return diskEnabled ? disk->read() : 0xFF; break;
        }

        return ports[port];
    }

    void out(uint8_t port, uint8_t value) {
        if(port >= ports.size()) return;

        switch(port) {
            case 0x01: /* TODO: terminal->printChar(value); */ break;
            case 0x11: /* TODO: drive selection*/ break;
            case 0x12: disk->setSector(value); break;
        }

        ports[port] = value;
    }
};
