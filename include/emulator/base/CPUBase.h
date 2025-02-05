#pragma once

#include "ICPU.h"

#include <deque>
#include <string>
#include <iostream>
#include <cstdint>
#include <memory>

template <typename Derived>
class CPUBase { // : public ICPU {
    public:
        std::deque<std::string> lastInstructions;

        void reset() { static_cast<Derived*>(this)->reset();}
        void step() { static_cast<Derived*>(this)->step(); }
        void decodeAndExecute(uint16_t opcode) { static_cast<Derived*>(this)->decodeAndExecute(opcode); }
        void decodeAndExecute(uint16_t opcode, uint8_t operand) { static_cast<Derived*>(this)->decodeAndExecute(opcode, operand); }
        void run() { static_cast<Derived*>(this)->run(); }

        void addLastInstruction(const std::string& instruction) {
            if(lastInstructions.size() >= 64) lastInstructions.pop_front();
            lastInstructions.push_back(instruction);
            executedInstructions++;
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

        unsigned int getExecutedInstructions() const { return executedInstructions; }

    protected:
        unsigned int executedInstructions = 0;
        int currentTimer = 0;
        int totalSpeeding = 0;
};
