#pragma once

#include <cstdint>

template <typename Derived>
class BIOSBase {
    public:
        void call(uint16_t callNumber) { static_cast<Derived*>(this)->call(callNumber); }
        void boot() { static_cast<Derived*>(this)->boot(); }
};
