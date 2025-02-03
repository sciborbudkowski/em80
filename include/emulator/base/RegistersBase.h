#pragma once

#include <cstdint>

template <typename Derived>
class RegistersBase {
    public:
        void reset() { static_cast<Derived*>(this)->reset(); }
};
