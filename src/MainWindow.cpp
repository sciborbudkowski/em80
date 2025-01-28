#include "MainWindow.h"
#include "EmulatorWindow.h"
#include "NewMachineDialog.h"
#include "MachinesManager.h"
#include "CPUType.h"

#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>

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

    loadMachines();
}

CPUType MainWindow::getSelectedCPUType() {
    return CPUType::I8080;
}

void MainWindow::createMachine() {
    NewMachineDialog dialog(this);

    if(dialog.exec() == QDialog::Accepted) {
        std::string machineName = dialog.getMachineName().toStdString();
        
        const auto& profiles = MachinesManager::getInstance().getAll();
        for (const auto& profile : profiles) {
            if (profile.name == machineName) {
                QMessageBox::warning(this, "Warning", "Machine with this name already exists.");
                return;
            }
        }

        MachineProfile profile = {
            machineName,
            0,
            false
        };
        MachinesManager::getInstance().add(profile);
        machinesList->addItem(dialog.getMachineName());
    }
}

void MainWindow::startMachine()
{
    this->hide();
    emulatorWindow = new EmulatorWindow(getSelectedCPUType());
    emulatorWindow->start();
    this->show();
}

void MainWindow::updateButtons()
{
    auto status = !machinesList->selectedItems().isEmpty();

    startButton->setEnabled(status);
    editButton->setEnabled(status);
    deleteButton->setEnabled(status);
}

void MainWindow::loadMachines() {
    const auto& profiles = MachinesManager::getInstance().getAll();
    for(const auto& profile : profiles) {
        machinesList->addItem(QString::fromStdString(profile.name));
    }
}

void MainWindow::onClosed() {
    delete emulatorWindow;
    emulatorWindow = nullptr;
}