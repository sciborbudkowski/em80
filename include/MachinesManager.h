#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

struct MachineProfile {
    std::string name;
    int cpuType;
    bool hasGraphics;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(MachineProfile, name, cpuType, hasGraphics);
};

class MachinesManager {
    public:
        static MachinesManager& getInstance();

        void add(const MachineProfile& profile);
        const std::vector<MachineProfile>& getAll() const;

        MachinesManager(const MachinesManager&) = delete;
        MachinesManager& operator=(const MachinesManager&) = delete;

        void saveToFile();
        void loadFromFile();

    private:
        MachinesManager();
        std::vector<MachineProfile> profiles;
        std::string configFile = "machines.json";
};
