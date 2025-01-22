#include "DiskController.h"

// DiskController::DiskController(const std::string& filename, size_t sectorSize) : sectorSize(sectorSize) {
//     loadDiskImage(filename);
// }

std::vector<uint8_t> DiskController::readSector(size_t sectorNumber) const {
    if(sectorNumber * sectorSize >= disk.size()) {
        throw std::out_of_range("Sector number out of range");
    }

    return std::vector<uint8_t>(
        disk.begin() + sectorNumber * sectorSize,
        disk.begin() + (sectorNumber + 1) * sectorSize
    );
}

void DiskController::writeSector(size_t sectorNumber, const std::vector<uint8_t>& data) {
    if(sectorNumber * sectorSize + data.size() > disk.size()) {
        throw std::out_of_range("Sector number out of range");
    }

    std::copy(data.begin(), data.end(), disk.begin() + sectorNumber * sectorSize);
}

void DiskController::loadDiskImage(const std::string& filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if(!file) {
        throw std::runtime_error("Failed to open disk image file");
    }

    disk.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}