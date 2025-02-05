#pragma once

#include "MemoryBase.h"

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

class DiskControllerInterface {
    public:
        virtual ~DiskControllerInterface() = default;

        virtual void setData(const std::vector<uint8_t>& ptrData) = 0;

        virtual void setSector(size_t sectorNumber) = 0;

        virtual uint8_t read() = 0;
        virtual void write(uint8_t value) = 0;

        virtual std::vector<uint8_t> readSector(uint8_t sectorNumber, uint8_t trackNumber, uint8_t sideNumber) = 0;
        virtual std::vector<uint8_t> readSector() = 0;

        virtual bool writeSector(uint8_t sectorNumber, uint8_t trackNumber, uint8_t sideNumber, const std::vector<uint8_t>& data) = 0;
        virtual bool writeSector(const std::vector<uint8_t>& data) = 0;

        virtual void reset() = 0;

        virtual void setDiskGeometry(const DiskGeometry& geometry) = 0;

        virtual void setCurrentDrive(size_t driveNumber) = 0;
        virtual void setCurrentSide(size_t sideNumber) = 0;
        virtual void setCurrentTrack(size_t trackNumber) = 0;
        virtual void setCurrentSector(size_t sectorNumber) = 0;

        virtual void determineDiskFormat() = 0;
};

class DiskControllerBase : public DiskControllerInterface {
    public:
        DiskControllerBase() : data() {};
        

        void setData(const std::vector<uint8_t>& ptrData) override { data = ptrData; }

        virtual void setSector(size_t sectorNumber) = 0;

        virtual uint8_t read() = 0;
        virtual void write(uint8_t value) = 0;

        virtual std::vector<uint8_t> readSector(uint8_t sectorNumber, uint8_t trackNumber, uint8_t sideNumber) = 0;
        virtual std::vector<uint8_t> readSector() = 0;

        virtual bool writeSector(uint8_t sectorNumber, uint8_t trackNumber, uint8_t sideNumber, const std::vector<uint8_t>& data) = 0;
        virtual bool writeSector(const std::vector<uint8_t>& data) = 0;

        virtual void reset() = 0;

        virtual void setDiskGeometry(const DiskGeometry& geometry) = 0;

        virtual void setCurrentDrive(size_t driveNumber) = 0;
        virtual void setCurrentSide(size_t sideNumber) = 0;
        virtual void setCurrentTrack(size_t trackNumber) = 0;
        virtual void setCurrentSector(size_t sectorNumber) = 0;

        virtual void determineDiskFormat() = 0;

    protected:
        std::vector<uint8_t> data;

        std::optional<DiskGeometry> geometry;
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
