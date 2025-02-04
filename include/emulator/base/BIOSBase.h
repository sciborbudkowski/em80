#pragma once

#include <cstdint>
#include <vector>
#include <fstream>

template <typename Derived>
class BIOSBase {
    public:
        void call(uint16_t callNumber) { static_cast<Derived*>(this)->call(callNumber); }
        void boot() { static_cast<Derived*>(this)->boot(); }

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
