#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>

class DiskController {
    public:
        DiskController() {};
        ~DiskController() = default;

        void setData(const std::vector<uint8_t>& data) {
            if(data.empty()) {
                totalSectors = 0;
                return;
            }
            totalSectors = data.size() / sectorSize;
        }

        void setSector(size_t sectorNumber) {
            if(sectorNumber >= totalSectors) {
                throw std::out_of_range("Sector number out of range");
            }

            currentSector = sectorNumber;
            sectorOffset = 0;
        }

        uint8_t read() {
            if(sectorOffset >= sectorSize) return 0xFF;
            return data[currentSector*sectorSize + sectorOffset++];
        }

        void write(uint8_t value) {
            if(sectorOffset >= sectorSize) return;
            data[currentSector*sectorSize + sectorOffset++] = value;
        }

    private:
        std::vector<uint8_t> data;
        size_t sectorSize = 128;
        size_t totalSectors;
        size_t currentSector = 0;
        size_t sectorOffset = 0;
};
