#pragma once

#include <QDialog>
#include <QString>

class QLineEdit;
class QComboBox;
class QPushButton;

class NewMachineDialog : public QDialog {
    Q_OBJECT

    public:
        explicit NewMachineDialog(QWidget* parent = nullptr);
        ~NewMachineDialog() = default;

        QString getMachineName() const;
        QString getProcessorType() const;

    private:
        QLineEdit* nameEdit;
        QComboBox* processorCombo;
        QPushButton* okButton;
        QPushButton* cancelButton;

        QString generateUniqueName(const QString* baseName);
};
