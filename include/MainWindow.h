#pragma once

#include "CPUType.h"
#include "MachinesManager.h"

#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>

class EmulatorWindow;

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr);
        ~MainWindow() = default;

        CPUType getSelectedCPUType();

    private:
        QPushButton* createButton;
        QPushButton* startButton;
        QPushButton* editButton;
        QPushButton* deleteButton;
        QPushButton* exitAppButton;
        QListWidget* machinesList;
        EmulatorWindow* emulatorWindow;
        MachinesManager machinesManager;

    private slots:
        void createMachine();
        void startMachine();
        void updateButtons();
        void removeMachine();
        void loadMachines();
        void onClosed();
};