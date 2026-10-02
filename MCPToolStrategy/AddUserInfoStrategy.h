#pragma once

#include "McpToolStrategy.h"

class UserDialogController;

// ---------------------------------------------------------
// Tool: AddUserInfoStrategy
// 負責接收使用者資訊，並寫入系統控制器的狀態中
// ---------------------------------------------------------

class AddUserInfoStrategy : public McpToolStrategy
{

public:
    // 透過建構子注入 Controller
    explicit AddUserInfoStrategy(UserDialogController* controller);
    ~AddUserInfoStrategy() override = default;

    QString getName() const override ;
    QString getDescription() const override;    
    QJsonObject getInputSchema() const override;    
    QJsonObject execute(const QJsonObject& arguments) override;

private:
    UserDialogController* m_userController;
};