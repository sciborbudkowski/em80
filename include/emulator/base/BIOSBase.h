#pragma once

#include <cstdint>
#include <vector>
#include <fstream>

enum CPMBIOSCALL : uint16_t {
    BOOT       =  0,
    WBOOT      =  1,
    CONST      =  2,
    CONIN      =  3,
    CONOUT     =  4,
    LIST       =  5,
    PUNCH      =  6,
    READER     =  7,
    HOME       =  8,
    SELDSK     =  9,
    SETTRK     = 10,
    SETSEC     = 11,
    SETDMA     = 12,
    READ       = 13,
    WRITE      = 14,
    // CP/M 2
    LISTST     = 15,
    SECTRAN    = 16,
    // CP/M 3
    CONOST     = 17,
    AUXIST     = 18,
    AUXOST     = 19,
    DEVTBL     = 20,
    DEVINI     = 21,
    DRVTBL     = 22,
    MULTIO     = 23,
    MOVE       = 24,
    TIME       = 25,
    SELMEM     = 26,
    SETBNK     = 27,
    XMOVE      = 28,
    USERF      = 29,
    RESERV1    = 30,
    RESERV2    = 31
};

class BIOSBase {
    public:
        virtual void call(CPMBIOSCALL callNumber) = 0;

        void loadBiosFromFile(const std::string& filename) {
            std::ifstream file(filename, std::ios::binary);
            if(file.is_open()) {
                file.read(reinterpret_cast<char*>(biosData.data()), biosData.size());
                file.close();
            }
        }

    protected:
        std::vector<uint8_t> biosData;
};
