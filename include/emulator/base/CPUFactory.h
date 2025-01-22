#pragma once

#include "CPUBase.h"
#include "CPUType.h"
#include "CPU8080.h"
#include "CPUZ80.h"

std::unique_ptr<CPUBase> createCPU(CPUType type) {
    switch(type) {
        case CPUType::I8080: return std::make_unique<CPU_8080::CPU8080>();
        case CPUType::Z80: return std::make_unique<CPU_Z80::CPUZ80>();
        default: throw std::invalid_argument("Invalid CPU type");
    }
}
