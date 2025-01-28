#include "MainWindow.h"
#include "EmulatorWindow.h"
#include "AddMachineDialog.h"
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
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::removeMachine);
    connect(exitAppButton, &QPushButton::clicked, this, &MainWindow::close);

    loadMachines();
}

CPUType MainWindow::getSelectedCPUType() {
    return CPUType::I8080;
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
    machinesList->clear();
    for(const auto& machine : machinesManager.getMachines()) {
        QListWidgetItem* item = new QListWidgetItem(machine.machineName);
        item->setData(Qt::UserRole, machine.processorType);
        machinesList->addItem(item);
    }
}

void MainWindow::onClosed() {
    delete emulatorWindow;
    emulatorWindow = nullptr;
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