#pragma once

#include <QString>
#include <QJsonObject>

// 定義 Strategy 介面 (代表 LLM 可以呼叫的 Function/Tool)
class McpToolStrategy
{
public:
    virtual ~McpToolStrategy() = default;
    virtual QString getName() const = 0;
    virtual QString getDescription() const = 0;
    virtual QJsonObject getInputSchema() const = 0;
    virtual QJsonObject execute(const QJsonObject& arguments) = 0;
};