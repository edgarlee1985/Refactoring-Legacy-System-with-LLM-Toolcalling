#pragma once
#include <QString>

#define MAX_DEVICES 100
#define MAX_USERS 50

struct DeviceConfig {
    bool isActive;
    int deviceType; // 0: 傳感器(Sensor), 1: 執行器(Actuator), 2: 繼電器(Relay)
    bool hasAutoMode;
    bool isCalibrated;
    QString deviceName;
    int thresholdValue;
};

struct UserInfo {
    QString username;
    bool isAdmin;
    bool canEditDevices;
    bool canRunOps;
};

// 全域靜態陣列宣告
extern DeviceConfig g_devices[MAX_DEVICES];
extern int g_deviceCount;

extern UserInfo g_users[MAX_USERS];
extern int g_userCount;