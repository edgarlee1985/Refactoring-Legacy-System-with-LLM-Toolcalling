#include "DeviceDialogController.h"
#include "GlobalData.h"

DeviceDialogController::DeviceDialogController()
{

}

DeviceDialogController::~DeviceDialogController()
{

}

DeviceConfig* DeviceDialogController::getDevice(int idx)
{
    return &g_devices[idx];
}

void DeviceDialogController::setDevice(int idx, DeviceConfig* deivce)
{
    assert(deivce);
    g_devices[idx].deviceName = deivce->deviceName;
    g_devices[idx].deviceType = deivce->deviceType;
    g_devices[idx].isActive = deivce->isActive;
    g_devices[idx].hasAutoMode = deivce->hasAutoMode;
    g_devices[idx].isCalibrated = deivce->isCalibrated;
    g_devices[idx].thresholdValue = deivce->thresholdValue;
}

void DeviceDialogController::deleteDevice(int idx)
{
    if (idx < 0 || idx >= g_deviceCount) return;

    // 將刪除目標後方的所有 Device 往前移
    for (int i = idx; i < g_deviceCount - 1; ++i)
    {
        g_devices[i] = g_devices[i + 1];
    }
    
    // 總數減 1
    g_deviceCount--;
}

void DeviceDialogController::addNewDevice(const DeviceConfig* neweivce)
{
    g_devices[g_deviceCount].deviceName = neweivce->deviceName;
    g_devices[g_deviceCount].deviceType = neweivce->deviceType;
    g_devices[g_deviceCount].isActive = neweivce->isActive;
    g_devices[g_deviceCount].hasAutoMode = neweivce->hasAutoMode;
    g_devices[g_deviceCount].isCalibrated = neweivce->isCalibrated;
    g_devices[g_deviceCount].thresholdValue = neweivce->thresholdValue;

    g_deviceCount++;
}