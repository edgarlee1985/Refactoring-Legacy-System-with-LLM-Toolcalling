#include "DeviceComputeStrategy.h"

#include <QJsonArray>

#include "src/OperationDialogController.h"
#include "src/UserDialogController.h"
#include "src/GlobalData.h"

DeviceComputeStrategy::DeviceComputeStrategy(OperationDialogController* opController, UserDialogController* userController)
: m_operationController(opController)
, m_userController(userController)
{
}

QString DeviceComputeStrategy::getName() const
{
    return "DeviceCompute";
}

QString DeviceComputeStrategy::getDescription() const
{
    return "以系統中存在的使用者身分，執行所有啟動中設備的運算及操作。請提供系統中存在的 username。";
}

QJsonObject DeviceComputeStrategy::getInputSchema() const
{
    // LLM 只需要輸入 username 作為檢索鍵值
    return QJsonObject{
        {"type", "object"},
        {"properties", QJsonObject{
            {"username", QJsonObject{{"type", "string"}}}
        }},
        {"required", QJsonArray{"username"}}
    };
}

QJsonObject DeviceComputeStrategy::execute(const QJsonObject& arguments)
{
    QString targetUsername = arguments["username"].toString();
    UserInfo* targetUser = nullptr;

    // 1. 透過 UserDialogController 在系統的 g_users 中查找真正的使用者指標
    int userCount = m_userController->getUserInfoCount();
    for (int i = 0; i < userCount; ++i) {
        UserInfo* user = m_userController->getUserInfo(i);
        if (user && user->username == targetUsername) { // 假設 UserInfo 有 username 屬性
            targetUser = user;
            break;
        }
    }

    // 2. 找不到使用者的錯誤處理[cite: 4, 6]
    if (!targetUser) {
        QJsonObject errorItem;
        errorItem["type"] = "text";
        errorItem["text"] = "[Error] User '" + targetUsername + "' not found in system state.";
        return QJsonObject{{"content", QJsonArray{errorItem}}};
    }

    // 3. 找到真正的指標後，傳遞給 OperationDialogController
    QString resultText = m_operationController->deviceCompute(targetUser);

    // 4. 回傳執行結果給 LLM
    QJsonObject contentItem;
    contentItem["type"] = "text";
    contentItem["text"] = resultText;

    return QJsonObject{
        {"content", QJsonArray{contentItem}}
    };
}