#pragma once

#include "DiskControllerBase.h"

class DiskControllerZ80 : public DiskControllerBase<DiskControllerZ80> {
    public:
        DiskControllerZ80() {};
        ~DiskControllerZ80() = default;

        void setData(const std::vector<uint8_t>& ptrData) {
            data = ptrData;
            if(ptrData.empty()) {
                if(geometry.has_value()) {
                    geometry->totalSectors = 0;
                }
                return;
            }
            if(geometry.has_value()) {
                geometry->totalSectors = data.size() / geometry->sectorSize;
            }
        }

        void setSector(size_t sectorNumber) {
            if(geometry.has_value() && sectorNumber >= geometry->totalSectors) {
                throw std::out_of_range("Sector number out of range");
            }

            currentSector = sectorNumber;
            if(geometry.has_value()) {
                geometry->sectorOffset = 0;
            }
        }

        uint8_t read() {
            if(geometry.has_value() && geometry->sectorOffset >= geometry->sectorSize) return 0xFF;
            return data[currentSector*geometry->sectorSize + geometry->sectorOffset++];
        }

        void write(uint8_t value) {
            if(geometry.has_value() && geometry->sectorOffset >= geometry->sectorSize) return;
            data[currentSector*geometry->sectorSize + geometry->sectorOffset++] = value;
        }

        std::vector<uint8_t> readSector(uint8_t sectorNumber, uint8_t trackNumber, uint8_t sideNumber) {
            if(sectorNumber < 1) {
                throw std::invalid_argument("Sector number must be greater than 0");
            }

            size_t absSector, offset;
            if(geometry.has_value()) {
                absSector = trackNumber * (geometry->tracksPerDisk * geometry->tracksPerDisk) + sideNumber * geometry->tracksPerDisk + (sectorNumber - 1);
                if(absSector >= geometry->totalSectors) {
                    throw std::out_of_range("Sector number out of range");
                }

                offset = absSector * geometry->sectorSize;
                if(offset + geometry->sectorSize > data.size()) {
                    throw std::out_of_range("Sector out of range");
                }
            }

            return std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + geometry->sectorSize);
        }

        void writeSector(uint8_t sectorNumber, uint8_t trackNumber, uint8_t sideNumber, const std::vector<uint8_t>& sectorData) {
            if(sectorNumber < 1) {
                throw std::invalid_argument("Sector number must be greater than 0");
            }

            size_t absSector, offset;
            if(geometry.has_value()) {
                absSector = trackNumber * (geometry->tracksPerDisk * geometry->tracksPerDisk) + sideNumber * geometry->tracksPerDisk + (sectorNumber - 1);
                if(absSector >= geometry->totalSectors) {
                    throw std::out_of_range("Sector number out of range");
                }

                offset = absSector * geometry->sectorSize;
                if(offset + geometry->sectorSize > data.size()) {
                    throw std::out_of_range("Sector out of range");
                }

                std::copy(sectorData.begin(), sectorData.begin() + geometry->sectorSize, data.begin() + offset);
            }
        }
};
