// #include "Memory8080.h"
// #include "IO8080.h"

// #include <iostream>

// Memory8080::Memory8080() : io(nullptr) {}

// void Memory8080::setIO(std::shared_ptr<IO8080> ioPtr) { io = ioPtr; }

// uint8_t Memory8080::read(uint16_t address) const {
//         if(address >= 0xF000) {
//             uint8_t port = address & 0xFF;
//             return io->in(port);
//         }
//         return ram[address];
// }

// void Memory8080::write(uint16_t address, uint8_t value) {
//         if(address >= 0xF000) {
//             uint8_t port = address & 0xFF;
//             io->out(port, value);
//         } else {
//             ram[address] = value;
//         }
// }

// void Memory8080::clear() {
//     std::fill(ram.begin(), ram.end(), 0);
// }

// bool Memory8080::loadProgram(uint16_t startAddress, const std::vector<uint8_t>& program) {
//     if(startAddress + program.size() > ram.size()) {
//         std::cerr << "Program too large to fit in memory" << std::endl;
//         return false;
//     }
//     std::copy(program.begin(), program.end(), ram.begin() + startAddress);
//     return true;
// }
