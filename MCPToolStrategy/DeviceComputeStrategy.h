#pragma once

#include "McpToolStrategy.h"

class OperationDialogController;
class UserDialogController;

class DeviceComputeStrategy : public McpToolStrategy
{
public:
    // 透過建構子注入 Controller
    explicit DeviceComputeStrategy(OperationDialogController* opController, UserDialogController* userController);
    ~DeviceComputeStrategy() override = default;

    QString getName() const override;
    QString getDescription() const override;
    QJsonObject getInputSchema() const override;
    QJsonObject execute(const QJsonObject& arguments) override;

private:
    OperationDialogController* m_operationController;
    UserDialogController* m_userController;
};