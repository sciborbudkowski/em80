#pragma once

#include "MemoryBase.h"

#include <QMessageBox>
#include <QString>
#include <fstream>

class Memory8080 : public MemoryBase {
    public:
        static constexpr size_t RAM_SIZE = 0x10000;

        Memory8080() : ram(RAM_SIZE, 0) {}

        uint8_t read(uint16_t address) const override { return ram[address]; }
        void write(uint16_t address, uint8_t value) override { ram[address] = value; }

        void clear() override { std::fill(ram.begin(), ram.end(), 0); }

        bool loadProgram(const std::string& filename, uint16_t startAddress) {
            std::ifstream file(filename, std::ios::in | std::ios::binary);
            if(!file) {
                QMessageBox::critical(nullptr, "Load Error", QString("Can not load program from file %1").arg(QString::fromStdString(filename)));
                return false;
            }

            file.seekg(0, std::ios::end);
            size_t fileSize = file.tellg();
            file.seekg(0, std::ios::beg);

            if(fileSize > RAM_SIZE - startAddress) {
                QMessageBox::critical(nullptr, "Load Error", QString("Program is too large to fit in memory"));
                return false;
            }

            file.read(reinterpret_cast<char*>(ram.data() + startAddress), fileSize);
            file.close();

            return true;
        }

    private:
        std::vector<uint8_t> ram;
};
