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
