#pragma once

#include <cstdint>
#include <vector>
#include <memory>

struct DiskControllerZ80;
struct TerminalZ80;

struct IOZ80 {
    std::vector<uint8_t> ports = std::vector<uint8_t>(0xFF, 0xFF);
    std::shared_ptr<TerminalZ80> terminal;
    std::shared_ptr<DiskControllerZ80> disk;
    bool diskEnabled = false;

    IOZ80();

    void setTerminal(std::shared_ptr<TerminalZ80> termPtr);
    void setDisk(std::shared_ptr<DiskControllerZ80> diskPtr);

    uint8_t in(uint8_t port) const;
    void out(uint8_t port, uint8_t value);
};
