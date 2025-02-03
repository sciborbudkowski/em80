#pragma once

#include "RegistersBase.h"
#include "MemoryBase.h"
#include "TerminalBase.h"
#include "DiskControllerBase.h"

#include <cstdint>
#include <vector>
#include <memory>

#define CCP         0xD400
#define BDOS        0xDC00
#define CBIOS       0xEA00
#define HDDISKS     2
#define STACK       0xEF00


template <typename RegistersType, typename MemoryType, typename TerminalType, typename DiskControllerType>
class CPMBIOS {
    public:
        CPMBIOS(RegistersType* registers, MemoryType* memory, TerminalType* terminal, DiskControllerType* diskController)
            : registers(registers), memory(memory), terminal(terminal), diskController(diskController) {}

        void call(uint16_t callNumber) { static_cast<Derived*>(this)->call(callNumber); }
        //     switch(callNumber) {
        //         case 0x00:      // CONIN
        //             registers->A = static_cast<uint8_t>(terminal->getLastChar());
        //             break;
        //         case 0x01:      // CONOUT
        //             terminal->printChar(static_cast<char>(registers->A));
        //             break;
        //         case 0x02:      // LISTOUT
        //             char ch = static_cast<char>(registers->A);
        //             terminal->printChar(ch);
        //             break;
        //         case 0x03:      // LISTSTR
        //             uint16_t addr = registers->HL();
        //             std::string str;
        //             char c = static_cast<char>(memory->read(addr));
        //             while(c != '$') {
        //                 str.push_back(c);
        //                 addr++;
        //                 c = static_cast<char>(memory->read(addr));
        //             }
        //             terminal->printString(str);
        //             break;
        //         case 0x04:      // CONSTAT
        //             bool status = terminal->getStatus();
        //             registers->A = (status ? 0xFF : 0x00);
        //             break;
        //         default:
        //             break;
        //     }
        // }

    private:
        RegistersType* registers;
        MemoryType* memory;
        TerminalType* terminal;
        DiskControllerType* diskController;

        void bootSystem() {};
        void readClock() {};
};
