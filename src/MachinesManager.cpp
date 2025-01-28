#include "MachinesManager.h"

#include <iostream>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStandardPaths>
#include <QDir>


MachinesManager::MachinesManager(QObject* parent) : QObject(parent) {
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    if(!dir.exists(configDir)) {
        dir.mkpath(configDir);
    }

    filePath = configDir + "/machines.json";
    loadMachines();
}

bool MachinesManager::loadMachines() {
    QFile file(filePath);
    if(!file.exists()) return true;
    if(!file.open(QIODevice::ReadOnly)) return false;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if(!doc.isArray()) return false;

    QJsonArray array = doc.array();
    machines.clear();
    for(const QJsonValue& val : array) {
        if(val.isObject()) {
            QJsonObject obj = val.toObject();
            QString processorType = obj["processorType"].toString();
            QString machineName = obj["machineName"].toString();
            machines.append(MachineProfile(processorType, machineName));
        }
    }

    return true;
}

bool MachinesManager::saveMachines() {
    // std::cout << "Saving machines to file: " << filePath.toStdString() << std::endl;
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly)) return false;

    QJsonArray array;
    for(const auto& machine : machines) {
        QJsonObject obj;
        obj["processorType"] = machine.processorType;
        obj["machineName"] = machine.machineName;
        array.append(obj);
    }

    QJsonDocument doc(array);
    file.write(doc.toJson());
    file.close();

    return true;
}

void MachinesManager::addMachine(const MachineProfile& machine) {
    machines.append(machine);
}

QVector<MachineProfile> MachinesManager::getMachines() const {
    return machines;
}

void MachinesManager::removeMachine(const QString& machineName) {
    for(int i=0; i<machines.size(); i++) {
        if(machines[i].machineName == machineName) {
            machines.removeAt(i);
            break;
        }
    }
}