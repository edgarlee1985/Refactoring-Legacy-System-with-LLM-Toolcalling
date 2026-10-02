#include "TestDeviceOperationStrategy.h"

#include <QJsonArray>
#include <QJsonObject>

#include "test/TestDeviceOperation.h"

TestDeviceOperationStrategy::TestDeviceOperationStrategy()
{

}

QString TestDeviceOperationStrategy::getName() const
{
    return "TestDeviceOperationCase";
}

QString TestDeviceOperationStrategy::getDescription() const
{ 
    return "執行設備操作測試案例，回傳整數錯誤碼 (0 為成功，非 0 為失敗)"; 
}

QJsonObject TestDeviceOperationStrategy::getInputSchema() const
{
    return QJsonObject{
        {"type", "object"},
        {"properties", QJsonObject()}
    };
}

QJsonObject TestDeviceOperationStrategy::execute(const QJsonObject& /*arguments*/)
{
    // 執行外部測試函數
    int retCode = TestDeviceOperationCase(); 
    
    QString resultText = (retCode == 0) ? "TEST_SUCCESS" : QString("TEST_FAILED with code: %1").arg(retCode);
    
    QJsonObject contentItem;
    contentItem["type"] = "text";
    contentItem["text"] = resultText;
    
    return QJsonObject{
        {"content", QJsonArray{contentItem}}
    };
}