#pragma once

struct DeviceConfig;

class DeviceDialogController
{
public:
    DeviceDialogController();
    ~DeviceDialogController();

    DeviceConfig* getDevice(int idx);
};