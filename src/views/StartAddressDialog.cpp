#include "StartAddressDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

#include <iostream>

StartAddressDialog::StartAddressDialog(QWidget* parent, uint16_t startAddress) : QDialog(parent), startAddress(startAddress) {
    std::cout << "StartAddressDialog constructor, startAddress: " << startAddress << std::endl;
    setWindowTitle("Start Address");

    convertStartAddressToHexString();

    auto* layout = new QVBoxLayout(this);

    auto* startAddressLabel = new QLabel("Start address:", this);
    startAddressEdit = new QLineEdit(startAddressAsHexString, this);

    auto* buttonsLayout = new QHBoxLayout();
    okButton = new QPushButton("OK", this);
    cancelButton = new QPushButton("Cancel", this);
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    layout->addWidget(startAddressLabel);
    layout->addWidget(startAddressEdit);
    layout->addLayout(buttonsLayout);

    connect(okButton, &QPushButton::clicked, this, &StartAddressDialog::onOkButtonClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

uint16_t StartAddressDialog::getStartAddress() const {
    return startAddress;
}

void StartAddressDialog::onOkButtonClicked() {
    QString text = startAddressEdit->text().trimmed();

    if(text.isEmpty()) {
        QMessageBox::warning(this, "Error", "Start address can not be empty.");
        return;
    }

    startAddress = text.toUInt(nullptr, 16);
    accept();
}

void StartAddressDialog::convertStartAddressToHexString() {
    std::cout << "convertStartAddressToHexString: " << startAddress << std::endl;
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%04X", startAddress);
    startAddressAsHexString = QString::fromUtf8(buffer);
    std::cout << "startAddressAsHexString: " << startAddressAsHexString.toStdString() << std::endl;
}

void StartAddressDialog::convertHexStringToStartAddress() {
    startAddress = startAddressAsHexString.toUInt(nullptr, 16);
}
