#pragma once

#include "McpToolStrategy.h"

class DeviceDialogController;

// ---------------------------------------------------------
// Tool: AddDeviceStrategy
// 負責接收設備參數，並註冊至系統控制器中
// ---------------------------------------------------------

class AddDeviceStrategy : public McpToolStrategy
{
public:
    // 透過建構子注入 Controller
    explicit AddDeviceStrategy(DeviceDialogController* controller);
    ~AddDeviceStrategy() override = default;

    QString getName() const override;
    QString getDescription() const override;
    QJsonObject getInputSchema() const override;
    QJsonObject execute(const QJsonObject& arguments) override;

private:
    DeviceDialogController* m_deviceController;
};