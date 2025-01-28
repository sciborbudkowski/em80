#pragma once

#include <QString>

class MachineProfile {
    public:
        QString processorType;
        QString machineName;

        MachineProfile() = default;
        MachineProfile(QString processorType, QString machineName) : processorType(processorType), machineName(machineName) {}
};
