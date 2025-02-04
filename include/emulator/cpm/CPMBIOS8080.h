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

        void call(BIOSCALL callNumber) {
            switch(callNumber) {
                case BIOSCALL::BOOT:
                    if(resetCPUCallback) {
                        resetCPUCallback();
                    }
                    break;
                case BIOSCALL::WBOOT:
                    break;
                case BIOSCALL::CONST:
                    registers->A = terminal->getStatus() ? 1 : 0;
                    break;
                case BIOSCALL::CONIN:
                    registers->A = terminal->getLastChar();
                    break;
                case BIOSCALL::CONOUT:
                    terminal->printChar(registers->C);
                    break;
                case BIOSCALL::LIST:
                    terminal->printChar(registers->C);
                    break;
                case BIOSCALL::PUNCH:
                    terminal->printChar(registers->C);
                    break;
                case BIOSCALL::READER:
                    registers->A = 26; // ^Z character, device not present
                    break;
                case BIOSCALL::HOME:
                    diskController->setCurrentDrive(0);
                    diskController->setCurrentSector(1);
                    break;
                case BIOSCALL::SELDSK:
                    diskController->setCurrentDrive(registers->C);
                    if(registers->C < HDDISKS) {
                        //registers->HL()
                    }
                    break;
                default:
                    break;
            }        
        }

    private:
        std::function<void()> resetCPUCallback;
};
