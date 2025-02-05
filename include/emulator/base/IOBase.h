#pragma once

#include "TerminalBase.h"
#include "DiskControllerBase.h"
#include <cstdint>
#include <memory>
class IOInterface {
    public:
        virtual ~IOInterface() = default;

        virtual uint8_t in(uint8_t port) const = 0;
        virtual void out(uint8_t port, uint8_t value) = 0;

        virtual TerminalBase& getTerminal() = 0;
        virtual DiskControllerBase& getDiskController() = 0;
};

class IOBase : public IOInterface {
    public:
        IOBase(std::unique_ptr<TerminalBase> terminal, std::unique_ptr<DiskControllerBase> disk)
            : terminal(std::move(terminal)), disk(std::move(disk)) {}

        virtual uint8_t in(uint8_t port) const = 0;
        virtual void out(uint8_t port, uint8_t value) = 0;

        TerminalBase& getTerminal() override {
            return* terminal;
        }
        
        DiskControllerBase& getDiskController() override {
            return* disk;
        }

    protected:
        std::unique_ptr<TerminalBase> terminal;
        std::unique_ptr<DiskControllerBase> disk;
};
