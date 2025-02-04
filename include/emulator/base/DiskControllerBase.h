#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <optional>

struct DiskGeometry {
    // Basic disk geometry
    size_t sectorSize;
    size_t sectorsPerTrack;
    size_t tracksPerDisk;
    size_t sectorOffset;
    size_t trackOffset;
    size_t reservedTracks;
    size_t sectorsPerBlock;
    size_t sectorsPerExtent;
    size_t totalExtents;
    size_t extentSize;

    // Calculated values
    size_t extentsPerSector;
    size_t trackSize;
    size_t discSize;
    size_t totalSectors;

    DiskGeometry(size_t sectorSize, size_t sectorsPerTrack, size_t tracksPerDisk, size_t sectorOffset, size_t trackOffset, size_t reservedTracks, size_t sectorsPerBlock, size_t sectorsPerExtent, size_t totalExtents, size_t extentSize)
        : sectorSize(sectorSize), sectorsPerTrack(sectorsPerTrack), tracksPerDisk(tracksPerDisk), sectorOffset(sectorOffset), trackOffset(trackOffset), reservedTracks(reservedTracks), sectorsPerBlock(sectorsPerBlock), sectorsPerExtent(sectorsPerExtent), totalExtents(totalExtents), extentSize(extentSize) {
        extentsPerSector = sectorSize/extentSize;
        trackSize = sectorsPerTrack*sectorSize;
        discSize = tracksPerDisk*trackSize;
        totalSectors = sectorsPerTrack*tracksPerDisk;
    }
};

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
        std::vector<uint8_t> readSector() { static_cast<Derived*>(this)->readSector(); }

        bool writeSector(uint8_t sectorNumber, uint8_t trackNumber, uint8_t sideNumber, const std::vector<uint8_t>& data) {
            return static_cast<Derived*>(this)->writeSector(sectorNumber, trackNumber, sideNumber, data);
        }
        bool writeSector(const std::vector<uint8_t>& data) { return static_cast<Derived*>(this)->writeSector(data); }

        void reset() { 
            sectorOffset = 0;
            currentSector = 0;
        }

        void setDiskGeometry(const DiskGeometry& geometry) { this->geometry = geometry; }

        void setCurrentDrive(size_t driveNumber) { this->currentDrive = driveNumber; }
        void setCurrentSide(size_t sideNumber) { this->currentSide = sideNumber; }
        void setCurrentTrack(size_t trackNumber) { this->currentTrack = trackNumber; }
        void setCurrentSector(size_t sectorNumber) { this->currentSector = sectorNumber; }

        void determineDiskFormat() {}

    protected:
        std::vector<uint8_t> data;

        DiskGeometry geometry;
        size_t currentSector = 0, currentSide = 0, currentTrack = 0, currentDrive = 0;
        std::optional<uint16_t> dphAddress;

        std::vector<uint8_t> sectorsTranslation = {
             1,  7, 13, 19,
            25,  5, 11, 17,
            23,  3,  9, 15,
            21,  2,  8, 14,
            20, 26,  6, 12,
            18, 24,  4, 10,
            16, 22
        };
};
