#pragma once

#include "MachinesManager.h"

#include <QDialog>

class QLineEdit;
class QComboBox;
class QPushButton;

class AddMachineDialog : public QDialog {
    Q_OBJECT

    public:
        explicit AddMachineDialog(QWidget* parent = nullptr);
        ~AddMachineDialog() = default;

        MachineProfile getMachine() const;

    private slots:
        void onOkButtonClicked();

    private:
        QLineEdit* nameEdit;
        QComboBox* processorCombo;
        QPushButton* okButton;
        QPushButton* cancelButton;

        MachineProfile profile;
};
