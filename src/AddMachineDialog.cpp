#include "AddMachineDialog.h"
#include "CPUBase.h"
#include "CPUType.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
AddMachineDialog::AddMachineDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Add New Machine");

    auto* layout = new QVBoxLayout(this);

    auto* processorLabel = new QLabel("Processor type:", this);
    processorCombo = new QComboBox(this);
    for(int i=0; i<=static_cast<int>(CPUType::I8086); i++) {
        auto type = static_cast<CPUType>(i);
        processorCombo->addItem(QString::fromStdString(getCPUName(type)), QVariant(i));
    }

    auto* nameLabel = new QLabel("Machine name:", this);
    nameEdit = new QLineEdit("New machine", this);

    auto* buttonsLayout = new QHBoxLayout();
    okButton = new QPushButton("OK", this);
    cancelButton = new QPushButton("Cancel", this);
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    layout->addWidget(processorLabel);
    layout->addWidget(processorCombo);
    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addLayout(buttonsLayout);

    connect(okButton, &QPushButton::clicked, this, &AddMachineDialog::onOkButtonClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

MachineProfile AddMachineDialog::getMachine() const {
    return profile;
}

void AddMachineDialog::onOkButtonClicked() {
    QString name = nameEdit->text().trimmed();
    QString processor = processorCombo->currentText();

    if (name.isEmpty() || processor.isEmpty()) {
        QMessageBox::warning(this, "Error", "Machine name can not be empty.");
        return;
    }

    profile = MachineProfile(processor, name);
    accept();
}
