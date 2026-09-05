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