#pragma once

#include "TerminalBase.h"

#include <unordered_map>
#include <cstdint>
#include <functional>
#include <stdexcept>

class IOBase {
    public:
        IOBase() : terminal(80, 25) {}
        virtual ~IOBase() = default;

        virtual uint8_t input(uint8_t port);
        virtual void output(uint8_t port, uint8_t value);

        void registerInputDevice(uint8_t port, std::function<uint8_t()> handler) { inputDevices[port] = handler; }
        void registerOutputDevice(uint8_t port, std::function<void(uint8_t)> handler) { outputDevices[port] = handler; }

        uint8_t input(uint8_t port) {
            if (inputDevices.find(port) != inputDevices.end()) {
                return inputDevices[port]();
            }
            throw std::runtime_error("Unmapped input port: " + std::to_string(port));
        }

        void output(uint8_t port, uint8_t value) {
            if(outputDevices.find(port) != outputDevices.end()) {
                outputDevices[port](value);
            } else {
                throw std::runtime_error("Unmapped output port: " + std::to_string(port));
            }
        }

        TerminalBase& getTerminal() { return terminal; }

    protected:
        TerminalBase terminal;

        std::unordered_map<uint8_t, std::function<uint8_t()>> inputDevices;
        std::unordered_map<uint8_t, std::function<void(uint8_t)>> outputDevices;
};
