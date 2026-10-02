#include "AddUserInfoStrategy.h"

#include <QJsonArray>

#include "src/UserDialogController.h"
#include "src/GlobalData.h"

AddUserInfoStrategy::AddUserInfoStrategy(UserDialogController* controller)
: m_userController(controller)
{

}

QString AddUserInfoStrategy::getName() const
{ 
    return "AddUserInfo";
}
    
QString AddUserInfoStrategy::getDescription() const
{ 
    return "新增或更新系統中的測試使用者資訊。"; 
}

QJsonObject AddUserInfoStrategy::getInputSchema() const
{
    return QJsonObject{
        {"type", "object"},
        {"properties", QJsonObject{
            {"username", QJsonObject{{"type", "string"}}},
            {"isAdmin", QJsonObject{{"type", "boolean"}}},
            {"canEditDevices", QJsonObject{{"type", "boolean"}}},
            {"canRunOps", QJsonObject{{"type", "boolean"}}}
        }},
        {"required", QJsonArray{"username"}}
    };
}

QJsonObject AddUserInfoStrategy::execute(const QJsonObject& arguments)
{
    UserInfo testUser;
    testUser.username = arguments["username"].toString();
    testUser.isAdmin = arguments["isAdmin"].toBool();
    testUser.canEditDevices = arguments["canEditDevices"].toBool();
    testUser.canRunOps = arguments["canRunOps"].toBool();
    
    // 將資料寫入系統狀態
    m_userController->initializeUserInfo(); 
    m_userController->addNewUserInfo(&testUser); 
    
    QJsonObject contentItem;
    contentItem["type"] = "text";
    contentItem["text"] = QString("User '%1' has been successfully added to the system state.").arg(testUser.username);
    
    return QJsonObject{
        {"content", QJsonArray{contentItem}}
    };
}