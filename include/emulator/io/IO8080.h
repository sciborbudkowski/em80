#pragma once

#include "IOBase.h"

#include <cstdint>
#include <vector>
#include <memory>

struct Terminal8080;
struct DiskController8080;

struct IO8080 final : public IOBase<IO8080, Terminal8080, DiskController8080> {
    std::vector<uint8_t> ports = std::vector<uint8_t>(256);
    bool diskEnabled = false;

    IO8080(std::shared_ptr<Terminal8080> terminal, std::shared_ptr<DiskController8080> disk)
        : IOBase(terminal, disk) {}

    uint8_t in(uint16_t port) const;
    void out(uint16_t port, uint8_t value);
};
