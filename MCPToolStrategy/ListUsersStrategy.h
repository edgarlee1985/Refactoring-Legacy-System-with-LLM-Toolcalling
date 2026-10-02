#pragma once

#include "McpToolStrategy.h"

class UserDialogController;

class ListUsersStrategy : public McpToolStrategy
{
public:
    // 透過建構子注入 Controller
    explicit ListUsersStrategy(UserDialogController* controller);
    ~ListUsersStrategy() override = default;

    QString getName() const override;
    QString getDescription() const override;
    QJsonObject getInputSchema() const override;
    QJsonObject execute(const QJsonObject& arguments) override;

private:
    UserDialogController* m_userController;
};