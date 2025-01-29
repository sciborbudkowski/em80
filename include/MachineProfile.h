#pragma once

#include <QString>

struct MachineProfile {
    QString processorType;
    QString machineName;

    MachineProfile() = default;
    MachineProfile(QString processorType, QString machineName) : processorType(processorType), machineName(machineName) {}
};
