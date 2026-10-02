#include "ListUsersStrategy.h"

#include <QJsonArray>
#include <QJsonObject>

#include "src/UserDialogController.h"
#include "src/GlobalData.h"

ListUsersStrategy::ListUsersStrategy(UserDialogController* controller)
: m_userController(controller)
{
}

QString ListUsersStrategy::getName() const
{
    return "ListUsers";
}

QString ListUsersStrategy::getDescription() const
{
    return "取得系統中所有已註冊的使用者列表，包含他們的使用者名稱 (username) 與權限資訊 (isAdmin, canRunOps 等)。";
}

QJsonObject ListUsersStrategy::getInputSchema() const
{
    // 這個指令不需要任何輸入參數[cite: 6]
    return QJsonObject{
        {"type", "object"},
        {"properties", QJsonObject()}
    };
}

QJsonObject ListUsersStrategy::execute(const QJsonObject& /*arguments*/)
{
    int userCount = m_userController->getUserInfoCount(); // 取得使用者總數
    
    if (userCount == 0) {
        QJsonObject contentItem;
        contentItem["type"] = "text";
        contentItem["text"] = "Currently, there are no users in the system.";
        return QJsonObject{{"content", QJsonArray{contentItem}}};
    }

    QString resultText = "System Users List:\n";
    
    // 遍歷所有使用者，將他們的資訊整理成易讀的文字給 LLM 看
    for (int i = 0; i < userCount; ++i) {
        UserInfo* user = m_userController->getUserInfo(i); // 取得個別 UserInfo 指標
        if (user) {
            resultText += QString("- Username: %1 (Admin: %2, CanEditDevices: %3, CanRunOps: %4)\n")
                              .arg(user->username)
                              .arg(user->isAdmin ? "True" : "False")
                              .arg(user->canEditDevices ? "True" : "False")
                              .arg(user->canRunOps ? "True" : "False"); // 將權限狀態如實呈現，幫助 LLM 判斷
        }
    }

    // 將整理好的字串打包回傳
    QJsonObject contentItem;
    contentItem["type"] = "text";
    contentItem["text"] = resultText;

    return QJsonObject{
        {"content", QJsonArray{contentItem}}
    };
}