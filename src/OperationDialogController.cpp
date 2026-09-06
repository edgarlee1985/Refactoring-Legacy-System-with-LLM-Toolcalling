#include "OperationDialogController.h"
#include "GlobalData.h"

OperationDialogController::OperationDialogController()
{

}

OperationDialogController::~OperationDialogController()
{

}

QString OperationDialogController::deviceCompute(const UserInfo* currentUser)
{
    QString output;
    // 權限檢查
    if (!currentUser->isAdmin && !currentUser->canRunOps) {
        output.append("[Error] " + currentUser->username + " lacks permission to run operations.");
        return output;
    }

    output.append("Starting operations as: " + currentUser->username + "...\n");
    
    for (int i = 0; i < g_deviceCount; ++i) {
        if (!g_devices[i].isActive) {
            output.append("Skipping inactive device: " + g_devices[i].deviceName);
            continue;
        }

        if (g_devices[i].deviceType == 0) { // Sensor
            if (g_devices[i].isCalibrated) {
                int mockData = rand() % 100;
                output.append("[Sensor] " + g_devices[i].deviceName + " reading is " + QString::number(mockData));
                if (mockData > g_devices[i].thresholdValue) {
                        output.append("   -> ALARM: Threshold exceeded!");
                }
            } else {
                output.append("[Sensor] " + g_devices[i].deviceName + " failed: Not Calibrated.");
            }
        } 
        else if (g_devices[i].deviceType == 1) { // Actuator
            if (g_devices[i].hasAutoMode && g_devices[i].thresholdValue > 50) {
                output.append("[Actuator] " + g_devices[i].deviceName + " auto-triggered with power " + QString::number(g_devices[i].thresholdValue));
            } else {
                output.append("[Actuator] " + g_devices[i].deviceName + " idle.");
            }
        }
        else if (g_devices[i].deviceType == 2) { // Relay
            output.append("[Relay] " + g_devices[i].deviceName + " clicked ON.");
        }
    }

    return output;
}