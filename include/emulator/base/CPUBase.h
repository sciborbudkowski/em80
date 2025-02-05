#pragma once

#include "RegistersBase.h"

#include <deque>
#include <string>
#include <iostream>
#include <cstdint>
#include <memory>

class CPUBase {
    public:
        CPUBase() = default;
        virtual ~CPUBase() = default;

        virtual void reset() = 0;
        virtual void step() = 0;
        virtual void decodeAndExecute(uint16_t opcode) = 0;
        virtual void decodeAndExecute(uint16_t opcode, uint8_t operand) { decodeAndExecute(opcode); }
        virtual void run() = 0;

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
        RegistersBase* registers;
        std::deque<std::string> lastInstructions;
        unsigned int executedInstructions = 0;
        int currentTimer = 0;
        int totalSpeeding = 0;
};
