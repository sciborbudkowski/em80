// #include "IO8080.h"
// #include "Terminal8080.h"
// #include "DiskController8080.h"

// uint8_t IO8080::in(uint16_t port) const {
//     if(port >= ports.size()) return 0xFF;

//     switch(port) {
//         case 0x02: return terminal->getLastChar(); break;
//         case 0x10: return diskEnabled ? 0x00 : 0xFF; break;
//         case 0x13: return diskEnabled ? disk->read() : 0xFF; break;
//     }

//     return ports[port];
// }

// void IO8080::out(uint16_t port, uint8_t value) {
//     if(port >= ports.size()) return;

//     switch(port) {
//         case 0x01: terminal->printChar(value); break;
//         case 0x11: /* TODO: drive selection*/ break;
//         case 0x12: disk->setSector(value); break;
//     }

//     ports[port] = value;
// }