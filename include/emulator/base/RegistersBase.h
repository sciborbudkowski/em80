#pragma once

#include <cstdint>
#include <variant>

#include "RegisterX.h"

class RegistersBase {
    public:
        virtual ~RegistersBase() = default;

        virtual void reset() = 0;
        virtual uint16_t getProgramCounter() const = 0;
        virtual void setProgramCounter(uint16_t value) = 0;

        virtual uint16_t getStackPointer() const = 0;
        virtual void setStackPointer(uint16_t value) = 0;

        virtual uint16_t getRegisterPair(int pair) const = 0;
        virtual void setRegisterPair(int pair, uint16_t value) = 0;

        virtual void dumpRegisters() const = 0;
};
