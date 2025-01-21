#pragma once

#include <vector>
#include <fstream>
#include <cstdint>

class DiskController {
    public:
        DiskController(const std::string& filename, size_t sectorSize) : sectorSize(sectorSize) {}
        std::vector<u_int8_t> readSector(size_t sectorNumber) const;
        void writeSector(size_t sectorNumber, const std::vector<uint8_t>& data);

    private:
        std::vector<uint8_t> disk;
        size_t sectorSize;

        void loadDiskImage(const std::string& filename);
};
