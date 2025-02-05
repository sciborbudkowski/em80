#pragma once

#include <cstdint>

template <typename RegistersType>
class RegistersBase {
    public:
        RegistersBase(RegistersType* registers) : registers(registers) {}
        virtual ~RegistersBase() = default;

        virtual void reset() = 0;

    protected:
        RegistersType registers;
};
