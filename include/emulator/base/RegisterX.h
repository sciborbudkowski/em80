#pragma once

#include <cstdint>
#include <string>

class Flag {
    public:
        bool value;
        std::string name;

        Flag(bool v = false, std::string n = "") : value(v), name(n) {}

        operator bool() const { return value; }
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
