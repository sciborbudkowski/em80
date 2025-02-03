#pragma once

#include "CPMBIOS.h"
#include "Registers8080.h"
#include "Memory8080.h"
#include "Terminal8080.h"
#include "DiskController8080.h"

#include <cstdint>
#include <string>
#include <functional>

class CPMBIOS8080 : public CPMBIOS<Registers8080, Memory8080, Terminal8080, DiskController8080> {
    public:
        CPMBIOS8080(Registers8080* registers, Memory8080* memory, Terminal8080* terminal, DiskController8080* diskController)
            : CPMBIOS(registers, memory, terminal, diskController) {}

        void setResetCPUCallback(std::function<void()> callback) { resetCPUCallback = callback; }

        void call(uint16_t callNumber) {
            switch(callNumber) {
                // BOOT
                case 0x00:
                    if(resetCPUCallback) {
                        resetCPUCallback();
                    }
                    break;
                // WBOOT
                case 0x01:

                default:
                    break;
            }        
        }

    private:
        std::function<void()> resetCPUCallback;
};
