#pragma once

struct DeviceConfig;

class DeviceDialogController
{
public:
    DeviceDialogController();
    ~DeviceDialogController();

    DeviceConfig* getDevice(int idx);
    void setDevice(int idx, DeviceConfig* deivce);

    void deleteDevice(int idx);

};