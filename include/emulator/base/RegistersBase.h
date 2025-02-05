#pragma once

#include <cstdint>

struct RegistersBase {
    RegistersBase() = default;
    virtual ~RegistersBase() = default;

    virtual void reset() = 0;
};
