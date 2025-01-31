#include "MainWindow.h"
#include "EmulatorWindow8080.h"
#include "CPU8080.h"
#include "AddMachineDialog.h"
#include "MachinesManager.h"
#include "CPUType.h"

#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    machinesList = new QListWidget(this);
    layout->addWidget(machinesList);

    createButton = new QPushButton("New...", this);
    startButton = new QPushButton("Run", this);
    startButton->setEnabled(false);
    editButton = new QPushButton("Edit...", this);
    editButton->setEnabled(false);
    deleteButton = new QPushButton("Delete...", this);
    deleteButton->setEnabled(false);
    exitAppButton = new QPushButton("Quit", this);

    layout->addWidget(createButton);
    layout->addWidget(startButton);
    layout->addWidget(editButton);
    layout->addWidget(deleteButton);
    layout->addWidget(exitAppButton);

    connect(createButton, &QPushButton::clicked, this, &MainWindow::createMachine);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startMachine);
    connect(machinesList, &QListWidget::itemSelectionChanged, this, &MainWindow::updateButtons);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::removeMachine);
    connect(exitAppButton, &QPushButton::clicked, this, &MainWindow::close);

    loadMachines();
}

CPUType MainWindow::getSelectedCPUType(int index) {
    // QString processorType = machinesList->currentItem()->data(Qt::UserRole).toString();
    std::cout << "trying to iterate machines" << std::endl;
    for(const auto& machine : machines) {
        std::cout << "mmmm" << std::endl;
        std::cout << machine.machineName.toStdString() << " " << machine.processorType.toStdString() << std::endl;
    }
    QString processorType = "I8080"; // machines[index].processorType;
    std::cout << "Selected CPU type: " << processorType.toStdString() << std::endl;
    if(processorType == "I8080") return CPUType::I8080;
    if(processorType == "Z80") return CPUType::Z80;
    if(processorType == "I8085") return CPUType::I8085;
    if(processorType == "I8086") return CPUType::I8086;
    if(processorType == "I8088") return CPUType::I8088;
    
    throw std::runtime_error("Unsupported CPU type!");
}

void MainWindow::createMachine() {
    AddMachineDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted) {
        MachineProfile machine = dialog.getMachine();
        machinesManager.addMachine(machine);
        if(machinesManager.saveMachines()) {
            loadMachines();
        } else {
            QMessageBox::warning(this, "Error", "Failed to save machines.");
        }
    }
}

void MainWindow::startMachine() {
    this->hide();

    CPUType cpuType = getSelectedCPUType(machinesList->currentRow());
    std::cout << "Starting machine with CPU " << cpuType << std::endl;

    switch(cpuType) {
        case CPUType::I8080: {
            auto cpu8080 = emulatorManager.getCPU8080();
            auto window8080 = emulatorManager.getEmulatorWindow8080();
            window8080->start();
            break;
        }
        case CPUType::Z80: {
            auto cpuZ80 = emulatorManager.getCPUZ80();
            auto windowZ80 = emulatorManager.getEmulatorWindowZ80();
            windowZ80->start();
            break;
        }
        // case CPUType::I8085: emulatorWindow = new EmulatorWindow(cpuType); break;
        // case CPUType::I8086: emulatorWindow = new EmulatorWindow(cpuType); break;
        // case CPUType::I8088: emulatorWindow = new EmulatorWindow(cpuType); break;
        default: throw std::runtime_error("Unsupported CPU type!");
    }

    this->show();
}

void MainWindow::updateButtons() {
    auto status = !machinesList->selectedItems().isEmpty();

    startButton->setEnabled(status);
    editButton->setEnabled(status);
    deleteButton->setEnabled(status);
}

void MainWindow::loadMachines() {
    machinesList->clear();
    for(const auto& machine : machinesManager.getMachines()) {
        QListWidgetItem* item = new QListWidgetItem(machine.machineName);
        item->setData(Qt::UserRole, machine.processorType);
        machinesList->addItem(item);
    }
}

void MainWindow::removeMachine() {
    QListWidgetItem* item = machinesList->currentItem();
    QString machineName = item->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm", "Are you sure you want to delete this machine?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        machinesManager.removeMachine(machineName);

        if(machinesManager.saveMachines()) {
            loadMachines();
        } else {
            QMessageBox::warning(this, "Error", "Failed to save machines.");
        }
    }
}