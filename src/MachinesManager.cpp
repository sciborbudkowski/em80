#include "MachinesManager.h"

#include <QMessageBox>

MachinesManager& MachinesManager::getInstance() {
    static MachinesManager instance;
    return instance;
}

MachinesManager::MachinesManager() {
    loadFromFile();
}

void MachinesManager::add(const MachineProfile& profile) {
    profiles.push_back(profile);
    saveToFile();
}

const std::vector<MachineProfile>& MachinesManager::getAll() const {
    return profiles;
}

void MachinesManager::saveToFile() {
    nlohmann::json data = profiles;

    for(const auto& profile : profiles) {
        data.push_back({
            {"name", profile.name},
            {"cpuType", profile.cpuType},
            {"hasGraphics", profile.hasGraphics}
        });
    }

    std::ofstream file(configFile);
    if(!file) {
        QMessageBox::critical(nullptr, "Save Error", QString("Nie można zapisać maszyn do pliku %1").arg(QString::fromStdString(configFile)));
        return;
    }

    file << data.dump(4);
}

void MachinesManager::loadFromFile() {
    profiles.clear();

    std::ifstream file(configFile);
    if(!file) return;

    nlohmann::json data;
    try {
        file >> data;
    } catch(const std::exception& e) {
        return;
    }

    for(const auto& entry : data) {
        MachineProfile profile = {
            entry["name"].get<std::string>(),
            entry["cpuType"].get<int>(),
            entry["hasGraphics"].get<bool>()
        };

        bool exists = false;
        for (const auto& existingProfile : profiles) {
            if (existingProfile.name == profile.name) {
                exists = true;
                break;
            }
        }

        if (!exists) profiles.push_back(profile);
    }
}