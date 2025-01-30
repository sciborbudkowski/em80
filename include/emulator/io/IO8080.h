// #pragma once

// #include "IOBase.h"

// class IO8080 : public IOBase {
//     public:
//         IO8080() {
//             registerInputDevice(0x00, [this]() { return terminal.getChar(); });
//             registerOutputDevice(0x01, [this](uint8_t value) { terminal.printChar(value); });
//         }

//         TerminalBase& getTerminal() { return terminal; }
// };
