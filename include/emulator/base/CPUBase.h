#pragma once

#include <deque>
#include <string>

template <typename Derived>
class CPUBase {
    public:
        std::deque<std::string>> lastInstructions;

        void reset() { static_cast<Derived*>(this)->reset();}
        void step() { static_cast<Derived*>(this)->step(); }
        void decodeAndExecute() { static_cast<Derived*>(this)->decodeAndExecute(); }
        void run() { static_cast<Derived*>(this)->run(); }

        void addLastInstruction(const std::string& instruction) {
            if(lastInstructions.size() >= 64) lastInstructions.pop_front();
            lastInstructions.push_back(instruction);
        }

        void dumpLastInstructions(int num = 8) const {
            int counter = 0;
            std::cout << "Last " << num << " instructions:\n";
            for(const auto& instr : lastInstructions) {
                std::cout << "[" << counter << "] " << instr << std::endl;
                counter++;
                if(counter >= num) break;
            }
        }
};

/*
#include "RegistersBase.h"
#include "MemoryBase.h"
#include "IOBase.h"

#include <deque>
#include <string>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <memory>

class CPUBase {
    public:
        virtual ~CPUBase() = default;

        virtual void reset() = 0;
        virtual void step() = 0;
        virtual void execute(uint8_t opcode) = 0;
        virtual void execute(uint8_t opcode, uint8_t subOpcode) = 0;
        virtual void run() = 0;

        std::string hex8(uint8_t value) const {
            std::ostringstream oss;
            oss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(value);
            return oss.str();
        }

        std::string hex16(uint16_t value) const {
            std::ostringstream oss;
            oss << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << value;
            return oss.str();
        }

        void setRunning(bool value) { running = value; }
        bool isRunning() const { return running; }

        uint16_t getAddress(uint8_t low, uint8_t high) const { return (high << 8) | low; }

        virtual void pushStack(uint16_t value) = 0;
        virtual uint16_t popStack() = 0;

        IOBase& getIO() { return *io; }
        MemoryBase& getMemory() { return *memory; }

        virtual void testOpcodes() = 0;

    protected:
        std::unique_ptr<RegistersBase> regs;
        std::unique_ptr<MemoryBase> memory;
        std::unique_ptr<IOBase> io;
        bool running;
};
*/