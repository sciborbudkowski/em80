// #pragma once

// #include "IOBase.h"

// class IOZ80 : public IOBase {
//     public:
//         IOZ80() {
//             registerInputDevice(0x00, [this]() { return terminal.getChar(); });
//             registerOutputDevice(0x01, [this](uint8_t value) { terminal.printChar(value); });
//         }
// };
