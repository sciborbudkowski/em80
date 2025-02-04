#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <optional>

template <typename Derived>
class DiskControllerBase {
    public:
        DiskControllerBase() : data() {};
        ~DiskControllerBase() = default;

        void setData(const std::vector<uint8_t>& ptrData) { static_cast<Derived*>(this)->setData(ptrData); }

        void setSector(size_t sectorNumber) { static_cast<Derived*>(this)->setSector(sectorNumber); }

        uint8_t read() { static_cast<Derived*>(this)->read(); }
        void write(uint8_t value) { static_cast<Derived*>(this)->write(value); }

        std::vector<uint8_t> readSector(uint8_t sectorNumber, uint8_t trackNumber, uint8_t sideNumber) {
            return static_cast<Derived*>(this)->readSector(sectorNumber, trackNumber, sideNumber);
        }

        void writeSector(uint8_t sectorNumber, uint8_t trackNumber, uint8_t sideNumber, const std::vector<uint8_t>& data) {
            static_cast<Derived*>(this)->writeSector(sectorNumber, trackNumber, sideNumber, data);
        }

        void reset() { 
            sectorOffset = 0;
            currentSector = 0;
        }

        void setDiskGeometry(size_t driveNumber, size_t sectorSize, size_t numberOfSides, size_t numberOfTracks) {
            this->currentDrive = driveNumber;
            this->sectorSize = sectorSize;
            this->numberOfSides = numberOfSides;
            this->numberOfTracks = numberOfTracks;
        }

        void setCurrentDrive(size_t driveNumber) { this->currentDrive = driveNumber; }
        void setCurrentTrack(size_t trackNumber) { this->currentTrack = trackNumber; }
        void setCurrentSector(size_t sectorNumber) { this->currentSector = sectorNumber; }

        void determineDiskFormat() {}

    protected:
        std::vector<uint8_t> data;
        size_t sectorSize = 128, numberOfSides = 2, numberOfTracks = 26;
        size_t currentSector = 0, currentSide = 0, currentTrack = 0, currentDrive = 0;
        size_t totalSectors = 0;
        size_t sectorOffset = 0;
        std::optional<uint16_t> dphAddress;
};
