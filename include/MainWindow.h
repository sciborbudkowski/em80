#pragma once

#include "CPUType.h"
#include "MachinesManager.h"
#include "MachineProfile.h"
#include "EmulatorManager.h"

#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>

class EmulatorWindow;

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr);
        ~MainWindow() = default;

        CPUType getSelectedCPUType(int index);

    private:
        QPushButton* createButton;
        QPushButton* startButton;
        QPushButton* editButton;
        QPushButton* deleteButton;
        QPushButton* exitAppButton;
        QListWidget* machinesList;
        MachinesManager machinesManager;
        EmulatorManager emulatorManager;
        
        std::vector<MachineProfile> machines;

    private slots:
        void createMachine();
        void startMachine();
        void updateButtons();
        void removeMachine();
        void loadMachines();
};
