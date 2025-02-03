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

enum class BIOSCALL : uint16_t {
    BOOT       = 0x00,
    WBOOT      = 0x01,
    CONSTAT    = 0x02,
    CONSIN     = 0x03,
    CONSOUT    = 0x04,
    LIST       = 0x05,
    PUNCH      = 0x06,
    READER     = 0x07,
    HOME       = 0x08,
    SELDISC    = 0x09,
    SETTRACK   = 0x0A,
    SETSECTOR  = 0x0B,
    SETDMA     = 0x0C,
    RDSECTOR   = 0x0D,
    WRSECTOR   = 0x0E,
    LISTSTAT   = 0x0F,
    SECTTRAN   = 0x10,
    OPENUNIX   = 0x11,
    CREATEUNIX = 0x12,
    RDWUNIX    = 0x13,
    WRUNIX     = 0x14,
    CLOSEUNIX  = 0x15,
    FINISH     = 0x16,
    DOTIME     = 0x17
};

template <typename RegistersType, typename MemoryType, typename TerminalType, typename DiskControllerType>
class CPMBIOS {
    public:
        CPMBIOS(RegistersType* registers, MemoryType* memory, TerminalType* terminal, DiskControllerType* diskController)
            : registers(registers), memory(memory), terminal(terminal), diskController(diskController) {}

        void call(uint16_t callNumber) { static_cast<Derived*>(this)->call(callNumber); }

    protected:
        RegistersType* registers;
        MemoryType* memory;
        TerminalType* terminal;
        DiskControllerType* diskController;

        void bootSystem() {};
        void readClock() {};
};
