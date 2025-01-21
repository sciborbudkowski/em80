#pragma once

#include <cstdint>
#include <variant>

#define _BC 0
#define _DE 1
#define _HL 2
#define _SP 3

using RegisterValue = std::variant<bool, uint8_t, uint16_t, uint32_t, uint64_t>;

class Flag {
    public:
        bool value;
        std::string name;

        Flag(bool v = false, std::string n = "") : value(v), name(n) {}

        operator bool() const { return value; }

        operator int() const { return static_cast<int>(value); }
};

class Register8 {
    public:
        uint8_t value;

        Register8(uint8_t v = 0) : value(v) {}

        operator int() const { return static_cast<int>(value); }
        
        Register8& operator=(int v) {
            value = static_cast<uint8_t>(v);
            return *this;
        }

        Register8& operator--() {
            --value;
            return *this;
        }

        Register8 operator--(int) {
            Register8 temp = *this;
            --value;
            return temp;
        }

        Register8& operator++() {
            ++value;
            return *this;
        }

        Register8 operator++(int) {
            Register8 temp = *this;
            ++value;
            return temp;
        }

        Register8& operator+=(int v) {
            value += static_cast<uint8_t>(v);
            return *this;
        }

        Register8& operator-=(int v) {
            value -= static_cast<uint8_t>(v);
            return *this;
        }

        Register8& operator&=(int v) {
            value &= v;
            return *this;
        }

        Register8& operator|=(int v) {
            value |= v;
            return *this;
        }

        Register8& operator^=(int v) {
            value ^= v;
            return *this;
        }
};

class Register16 {
    public:
        uint16_t value;

        Register16(uint16_t v = 0) : value(v) {}

        operator int() const { return static_cast<int>(value); }
        
        Register16& operator=(int v) {
            value = static_cast<uint16_t>(v);
            return *this;
        }

        Register16& operator--() {
            --value;
            return *this;
        }

        Register16 operator--(int) {
            Register16 temp = *this;
            --value;
            return temp;
        }

        Register16& operator++() {
            ++value;
            return *this;
        }

        Register16 operator++(int) {
            Register16 temp = *this;
            ++value;
            return temp;
        }

        Register16& operator+=(int v) {
            value += static_cast<uint16_t>(v);
            return *this;
        }

        Register16& operator-=(int v) {
            value -= static_cast<uint16_t>(v);
            return *this;
        }
};

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

        virtual std::map<std::string, RegisterValue> getRegisters() const = 0;

        virtual void dumpRegisters() const = 0;
};
