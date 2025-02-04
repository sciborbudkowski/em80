#include "DiskControllerBase.h"

class DiskController8080 final : public DiskControllerBase<DiskController8080> {
    public:
        DiskController8080() = default;
        ~DiskController8080() = default;

        void setData(const std::vector<uint8_t>& data) { this->data = data; }

        void setSector(size_t sectorNumber) {
            if(sectorNumber >= geometry.totalSectors) {
                throw std::out_of_range("Sector number out of range");
            }

            currentSector = sectorNumber;
            geometry.sectorOffset = 0;
        }

        uint8_t read() {
            if(geometry.sectorOffset >= geometry.sectorSize) return 0xFF;
            return data[currentSector*geometry.sectorSize + geometry.sectorOffset++];
        }

        void write(uint8_t value) {
            if(geometry.sectorOffset >= geometry.sectorSize) return;
            data[currentSector*geometry.sectorSize + geometry.sectorOffset++] = value;
        }

        std::vector<uint8_t> readSector(uint8_t sectorNumber, uint8_t trackNumber, uint8_t sideNumber) {
            if(sectorNumber < 1) {
                throw std::invalid_argument("Sector number must be greater than 0");
            }

            size_t absSector = trackNumber * (geometry.tracksPerDisk * geometry.tracksPerDisk) + sideNumber * geometry.tracksPerDisk + (sectorNumber - 1);
            if(absSector >= geometry.totalSectors) {
                throw std::out_of_range("Sector number out of range");
            }

            size_t offset = absSector * geometry.sectorSize;
            if(offset + geometry.sectorSize > data.size()) {
                throw std::out_of_range("Sector out of range");
            }

            return std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + geometry.sectorSize);
        }

        std::vector<uint8_t> readSector() { return readSector(currentSector, currentTrack, currentSide); }

        bool writeSector(uint8_t sectorNumber, uint8_t trackNumber, uint8_t sideNumber, const std::vector<uint8_t>& sectorData) {
            if(sectorNumber < 1) {
                return false;
            }

            size_t absSector = trackNumber * (geometry.tracksPerDisk * geometry.tracksPerDisk) + sideNumber * geometry.tracksPerDisk + (sectorNumber - 1);
            if(absSector >= geometry.totalSectors) {
                return false;
            }

            size_t offset = absSector * geometry.sectorSize;
            if(offset + geometry.sectorSize > data.size()) {
                return false;
            }

            std::copy(sectorData.begin(), sectorData.begin() + geometry.sectorSize, this->data.begin() + offset);
            return true;
        }

        bool writeSector(const std::vector<uint8_t>& sectorData) { return writeSector(currentSector, currentTrack, currentSide, sectorData); }

    private:
        DiskGeometry cpmDiskGeometry = DiskGeometry(128, 16, 77, 1, 0, 2, 8, 128, 32, 64);
};
