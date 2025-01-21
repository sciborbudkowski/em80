#pragma once

#include <string>

enum class CPUType {
    I8080,
    I8085,
    Z80,
    I8088,
    I8086
};

std::string getCPUName(CPUType type) {
    switch(type) {
        case CPUType::I8080: return "Intel 8080";
        case CPUType::I8085: return "Intel 8085";
        case CPUType::Z80: return "Zilog Z80";
        case CPUType::I8088: return "Intel 8088";
        case CPUType::I8086: return "Intel 8086";
    }
}
