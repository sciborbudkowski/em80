#pragma once

#include <string>
#include <stdexcept>

enum CPUType {
    I8080,
    I8085,
    Z80,
    I8088,
    I8086,
    NOT_SET
};

inline std::string getCPUName(CPUType type) {
    switch(type) {
        case CPUType::I8080: return "Intel 8080";
        case CPUType::I8085: return "Intel 8085";
        case CPUType::Z80: return "Zilog Z80";
        case CPUType::I8088: return "Intel 8088";
        case CPUType::I8086: return "Intel 8086";
    }

    throw std::runtime_error("CPU type not set. Call EmulatorManager::setCPU(ICPU*) first.");
}
