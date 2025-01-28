#pragma once

#include <QObject>
#include <QVector>
#include <QString>

#include "MachineProfile.h"

class MachinesManager : public QObject {
    Q_OBJECT

    public:
        explicit MachinesManager(QObject* parent = nullptr);

        bool loadMachines();
        bool saveMachines();
        void removeMachine(const QString& machineName);
        void addMachine(const MachineProfile& profile);
        QVector<MachineProfile> getMachines() const;

    private:
        QVector<MachineProfile> machines;
        QString filePath;
};
