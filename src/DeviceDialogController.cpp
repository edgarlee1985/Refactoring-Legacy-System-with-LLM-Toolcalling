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