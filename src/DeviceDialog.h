#pragma once
#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QListWidget>
#include "GlobalData.h"

class DeviceDialogController;

class DeviceDialog : public QDialog {
    Q_OBJECT
public:
    explicit DeviceDialog(QWidget *parent = nullptr);

private slots:
    void onSaveClicked();
    void onDeviceSelected();
    void onUpdateClicked();
    void onDeleteClicked();
    void refreshDeviceList();

private:
    QLineEdit* txtName;
    QComboBox* comboType;
    QCheckBox* chkActive;
    QCheckBox* chkAutoMode;
    QCheckBox* chkCalibrated;
    QLineEdit* txtThreshold;
    QListWidget* listDevices;
    QPushButton* btnSave;
    QPushButton* btnUpdate;
    QPushButton* btnDelete;

    DeviceDialogController* deviceController;
};