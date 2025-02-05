#pragma once

#include <cstdint>
#include <vector>
#include <fstream>

template <typename RegistersType>
class BIOSBase {
    public:
        virtual void call(uint16_t callNumber) = 0;
        virtual void boot() = 0;

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
