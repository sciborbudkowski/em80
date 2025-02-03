#include "DiskControllerBase.h"

class DiskControllerZ80 : public DiskControllerBase<DiskControllerZ80> {
    public:
        DiskControllerZ80() {};
        ~DiskControllerZ80() = default;

        void setData(const std::vector<uint8_t>& ptrData) {
            data = ptrData;
            if(ptrData.empty()) {
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

        std::vector<uint8_t> readSector(uint8_t sectorNumber, uint8_t trackNumber, uint8_t sideNumber) {
            if(sectorNumber < 1) {
                throw std::invalid_argument("Sector number must be greater than 0");
            }

            size_t absSector = trackNumber * (numberOfTracks * numberOfTracks) + sideNumber * numberOfTracks + (sectorNumber - 1);
            if(absSector >= totalSectors) {
                throw std::out_of_range("Sector number out of range");
            }

            size_t offset = absSector * sectorSize;
            if(offset + sectorSize > data.size()) {
                throw std::out_of_range("Sector out of range");
            }

            return std::vector<uint8_t>(data.begin() + offset, data.begin() + offset + sectorSize);
        }

        void writeSector(uint8_t sectorNumber, uint8_t trackNumber, uint8_t sideNumber, const std::vector<uint8_t>& sectorData) {
            if(sectorNumber < 1) {
                throw std::invalid_argument("Sector number must be greater than 0");
            }

            size_t absSector = trackNumber * (numberOfTracks * numberOfTracks) + sideNumber * numberOfTracks + (sectorNumber - 1);
            if(absSector >= totalSectors) {
                throw std::out_of_range("Sector number out of range");
            }

            size_t offset = absSector * sectorSize;
            if(offset + sectorSize > data.size()) {
                throw std::out_of_range("Sector out of range");
            }

            std::copy(sectorData.begin(), sectorData.begin() + sectorSize, data.begin() + offset);
        }
};
