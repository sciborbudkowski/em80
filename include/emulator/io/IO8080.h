#pragma once

#include <cstdint>
#include <vector>
#include <memory>

struct Terminal8080;
struct DiskController8080;
struct IO8080 {
    std::vector<uint8_t> ports = std::vector<uint8_t>(256);
    std::shared_ptr<Terminal8080> terminal;
    std::shared_ptr<DiskController8080> disk;
    bool diskEnabled = false;

    IO8080();

    void setTerminal(std::shared_ptr<Terminal8080> termPtr);
    void setDisk(std::shared_ptr<DiskController8080> diskPtr);

    uint8_t in(uint16_t port) const;
    void out(uint16_t port, uint8_t value);
};
