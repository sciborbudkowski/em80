#pragma once

#include "IOBase.h"
#include "Terminal8080.h"
#include "DiskController8080.h"

#include <cstdint>
#include <vector>
#include <memory>

struct IO8080 final : public IOBase {
    std::vector<uint8_t> ports = std::vector<uint8_t>(256);
    bool diskEnabled = false;

    IO8080(std::shared_ptr<Terminal8080> terminal, std::shared_ptr<DiskController8080> disk) : IOBase(std::make_unique<Terminal8080>(terminal), std::make_unique<DiskController8080>(disk)) {}

    uint8_t in(uint8_t port) const override {
        if(port >= ports.size()) return 0xFF;

        switch(port) {
            case 0x02: return terminal->getLastChar(); break;
            case 0x10: return diskEnabled ? 0x00 : 0xFF; break;
            case 0x13: return diskEnabled ? disk->read() : 0xFF; break;
        }

        return ports[port];
    }

    void out(uint8_t port, uint8_t value) override {
        if(port >= ports.size()) return;

        switch(port) {
            case 0x01: terminal->printChar(value); break;
            case 0x11: /* TODO: drive selection*/ break;
            case 0x12: disk->setSector(value); break;
        }

        ports[port] = value;
    }
};
