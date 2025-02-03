#pragma once

#include <QDialog>
#include <cstdint>

class QLineEdit;
class QPushButton;

class StartAddressDialog : public QDialog {
    Q_OBJECT

    public:
        explicit StartAddressDialog(QWidget* parent = nullptr, uint16_t startAddress = 0x0100);
        ~StartAddressDialog() = default;

        uint16_t getStartAddress() const;

    private slots:
        void onOkButtonClicked();

    private:
        QLineEdit* startAddressEdit;
        QPushButton* okButton;
        QPushButton* cancelButton;

        uint16_t startAddress;
        QString startAddressAsHexString;

        void convertStartAddressToHexString();
        void convertHexStringToStartAddress();
};
