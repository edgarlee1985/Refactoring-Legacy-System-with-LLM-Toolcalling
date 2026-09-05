#include "DeviceDialog.h"
#include "DeviceDialogController.h"

DeviceDialog::DeviceDialog(QWidget *parent)
: QDialog(parent)
{
    deviceController = new DeviceDialogController;

    setWindowTitle("Device Configuration (Legacy)");
    QVBoxLayout* layout = new QVBoxLayout(this);

    listDevices = new QListWidget(this);

    txtName = new QLineEdit(this);
    txtName->setPlaceholderText("Device Name");
    
    comboType = new QComboBox(this);
    comboType->addItems({"Sensor", "Actuator", "Relay"});
    
    chkActive = new QCheckBox("Is Active", this);
    chkAutoMode = new QCheckBox("Enable Auto Mode", this);
    chkCalibrated = new QCheckBox("Is Calibrated", this);
    
    txtThreshold = new QLineEdit(this);
    txtThreshold->setPlaceholderText("Threshold Value (int)");

    btnSave = new QPushButton("Add New Device", this);
    
    btnUpdate = new QPushButton("Update Selected Device", this);
    btnUpdate->setEnabled(false);

    btnDelete = new QPushButton("Delete Selected Device", this);
    btnDelete->setEnabled(false);

    layout->addWidget(listDevices);
    layout->addWidget(txtName);
    layout->addWidget(comboType);
    layout->addWidget(chkActive);
    layout->addWidget(chkAutoMode);
    layout->addWidget(chkCalibrated);
    layout->addWidget(txtThreshold);
    layout->addWidget(btnSave);
    layout->addWidget(btnUpdate);
    layout->addWidget(btnDelete);

    connect(btnSave, &QPushButton::clicked, this, &DeviceDialog::onSaveClicked);
    connect(listDevices, &QListWidget::itemSelectionChanged, this, &DeviceDialog::onDeviceSelected);
    connect(btnUpdate, &QPushButton::clicked, this, &DeviceDialog::onUpdateClicked);
    connect(btnDelete, &QPushButton::clicked, this, &DeviceDialog::onDeleteClicked);

    // 視窗建立時，先載入一次目前的列表
    refreshDeviceList();
}

void DeviceDialog::onDeviceSelected()
{
    int idx = listDevices->currentRow();
    if (idx < 0 || idx >= g_deviceCount) {
        btnUpdate->setEnabled(false);
        return;
    }

    btnUpdate->setEnabled(true);
    btnDelete->setEnabled(true);

    DeviceConfig* device = deviceController->getDevice(idx);

    // 回填 UI 狀態
    txtName->setText(device->deviceName);
    comboType->setCurrentIndex(device->deviceType);
    chkActive->setChecked(device->isActive);
    chkAutoMode->setChecked(device->hasAutoMode);
    chkCalibrated->setChecked(device->isCalibrated);
    txtThreshold->setText(QString::number(g_devices[idx].thresholdValue));
}

void DeviceDialog::onUpdateClicked()
{
    int idx = listDevices->currentRow();
    if (idx < 0 || idx >= g_deviceCount) return;

    DeviceConfig* deivce = new DeviceConfig;
    deivce->deviceName = txtName->text();
    deivce->deviceType = comboType->currentIndex();
    deivce->isActive = chkActive->isChecked();
    deivce->hasAutoMode = chkAutoMode->isChecked();
    deivce->isCalibrated = chkCalibrated->isChecked();
    deivce->thresholdValue = txtThreshold->text().toInt();
    deviceController->setDevice(idx, deivce);

    QMessageBox::information(this, "Updated", "Device updated successfully.");

    refreshDeviceList();
    
    // 重新選中剛剛更新的項目
    listDevices->setCurrentRow(idx);
}

void DeviceDialog::onDeleteClicked()
{
    int idx = listDevices->currentRow();
    if (idx < 0 || idx >= g_deviceCount) return;

    deviceController->deleteDevice(idx);

    QMessageBox::information(this, "Deleted", "Device deleted successfully.");

    refreshDeviceList();
    
    // 清空狀態與鎖定按鈕
    txtName->clear();
    comboType->setCurrentIndex(0);
    chkActive->setChecked(false);
    chkAutoMode->setChecked(false);
    chkCalibrated->setChecked(false);
    txtThreshold->clear();
    btnUpdate->setEnabled(false);
    btnDelete->setEnabled(false);
}

void DeviceDialog::onSaveClicked()
{
    if (g_deviceCount >= MAX_DEVICES)
    {
        QMessageBox::warning(this, "Error", "Device array is full!");
        return;
    }

    DeviceConfig* deivce = new DeviceConfig;
    deivce->deviceName = txtName->text();
    deivce->deviceType = comboType->currentIndex();
    deivce->isActive = chkActive->isChecked();
    deivce->hasAutoMode = chkAutoMode->isChecked();
    deivce->isCalibrated = chkCalibrated->isChecked();
    deivce->thresholdValue = txtThreshold->text().toInt();

    deviceController->addNewDevice(deivce);

    QMessageBox::information(this, "Saved", "Device appended to g_devices[]");

    // 重新整理 UI 列表
    refreshDeviceList();

    // 清空輸入框狀態，準備輸入下一個 Device
    txtName->clear();
    comboType->setCurrentIndex(0);
    chkActive->setChecked(false);
    chkAutoMode->setChecked(false);
    chkCalibrated->setChecked(false);
    txtThreshold->clear();
}

void DeviceDialog::refreshDeviceList()
{
    // 清空舊畫面資料
    listDevices->clear();
    
    QStringList typeNames = {"Sensor", "Actuator", "Relay"};
    
    // 迴圈讀取全域陣列來重建清單
    for (int i = 0; i < g_deviceCount; ++i) {
        QString devInfo = g_devices[i].deviceName;
        
        int typeIdx = g_devices[i].deviceType;
        QString typeStr = (typeIdx >= 0 && typeIdx < typeNames.size()) ? typeNames[typeIdx] : "Unknown";
        
        devInfo += " [" + typeStr + "] ";
        devInfo += g_devices[i].isActive ? "(Active)" : "(Inactive)";
        devInfo += " | Thr: " + QString::number(g_devices[i].thresholdValue);
        
        // 將組合好的字串加入 QListWidget
        listDevices->addItem(devInfo);
    }
}