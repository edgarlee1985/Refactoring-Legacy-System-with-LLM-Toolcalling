#pragma once

#include "McpToolStrategy.h"

class TestDeviceOperationStrategy : public McpToolStrategy
{
public:
    TestDeviceOperationStrategy();
    ~TestDeviceOperationStrategy() override = default;

    QString getName() const override;
    QString getDescription() const override;
    QJsonObject getInputSchema() const override;
    QJsonObject execute(const QJsonObject& /*arguments*/);
};