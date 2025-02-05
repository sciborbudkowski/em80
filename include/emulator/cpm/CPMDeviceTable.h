#pragma once

#include "MemoryBase.h"

#include <cstdint>
#include <cstddef>
#include <memory>

struct CPMDevice {
    char name[6];
    uint8_t mode;
    uint8_t baudrate;
};

class CPMDeviceTable {
    public:
        ~CPMDeviceTable() = default;

        void createDeviceTable(MemoryInterface& memory) {
            CPMDevice localDeviceTable[CPMDeviceTable::CPM_MAX_DEVICES] = {
                {"CON", 0x03, 9},
                {"KBD", 0x01, 8}
            };

            const uint16_t address = getAddress();
            for(size_t i=0; i<CPMDeviceTable::CPM_MAX_DEVICES; i++) {
                uint16_t destAddress = address + i*sizeof(CPMDevice);
                uint8_t value = reinterpret_cast<uint8_t>(&localDeviceTable[i]);
                memory.write(destAddress, value);
            }
        }

        uint16_t getAddress() {
            return 0x0900;
        }

    private:
        static constexpr size_t CPM_MAX_DEVICES = 15;
        CPMDevice deviceTable[CPM_MAX_DEVICES];
};
