/*
#include "Disk.h"
#include "Memory.h"

#include <fstream>
#include <iostream>
#include <QMessageBox>
#include <QString>

uint8_t Disk::read(uint16_t address) {
    if(!loaded || address >= disk.size()) {
        QMessageBox::critical(nullptr, "Disk Error", QString("Disk read error at address %1").arg(address));
        return 0xFF;
    }
    return disk[address];
}

void Disk::write(uint16_t address, uint8_t value) {
    disk[address] = value;
}

void Disk::loadDiskFromFile(const std::string& filename, Memory& memory, size_t startAddress) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if(!file) {
        QMessageBox::critical(nullptr, "Load Error", QString("Can not load disk image from file %1").arg(QString::fromStdString(filename)));
        return;
    }

    //file.read(reinterpret_cast<char*>(disk.data()), DISK_SIZE);
    disk = std::vector<uint8_t>(std::istreambuf_iterator<char>(file), {});
    if(startAddress + disk.size() > memory.ram.size()) {
        QMessageBox::critical(nullptr, "Load Error", QString("Disk image is too large to fit in memory"));
        return;
    }
    std::copy(disk.begin(), disk.end(), memory.ram.begin() + startAddress);
    loaded = true;

    file.close();
}

void Disk::saveDiskToFile(const std::string& filename) {
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if(!file) {
        QMessageBox::critical(nullptr, "Save Error", QString("Can not save disk image to file %1").arg(QString::fromStdString(filename)));
        return;
    }

    file.write(reinterpret_cast<const char*>(disk.data()), DISK_SIZE);
    file.close();
}
*/