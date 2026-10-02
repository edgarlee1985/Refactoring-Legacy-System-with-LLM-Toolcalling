#include "AddDeviceStrategy.h"

#include <QJsonArray>

#include "src/DeviceDialogController.h"
#include "src/GlobalData.h"

// 透過建構子注入有狀態的 Controller
AddDeviceStrategy::AddDeviceStrategy(DeviceDialogController* controller)
: m_deviceController(controller)
{

}

QString AddDeviceStrategy::getName() const
{
    return "AddDevice";
}

QString AddDeviceStrategy::getDescription() const 
{ 
    return "新增設備設定至系統中。"; 
}

QJsonObject AddDeviceStrategy::getInputSchema() const
{
    return QJsonObject{
        {"type", "object"},
        {"properties", QJsonObject{
            {"deviceName", QJsonObject{{"type", "string"}}},
            {"deviceType", QJsonObject{{"type", "integer"}}},
            {"isActive", QJsonObject{{"type", "boolean"}}},
            {"isCalibrated", QJsonObject{{"type", "boolean"}}},
            {"thresholdValue", QJsonObject{{"type", "integer"}}},
            {"hasAutoMode", QJsonObject{{"type", "boolean"}}}
        }},
        {"required", QJsonArray{"deviceName", "deviceType", "thresholdValue"}}
    };
}

QJsonObject AddDeviceStrategy::execute(const QJsonObject& arguments)
{
    DeviceConfig dev;
    dev.deviceName = arguments["deviceName"].toString();
    dev.deviceType = arguments["deviceType"].toInt();
    dev.isActive = arguments["isActive"].toBool();
    dev.isCalibrated = arguments["isCalibrated"].toBool();
    dev.thresholdValue = arguments["thresholdValue"].toInt();
    dev.hasAutoMode = arguments["hasAutoMode"].toBool();
    
    // 將設備寫入系統狀態
    m_deviceController->addNewDevice(&dev);
    
    QJsonObject contentItem;
    contentItem["type"] = "text";
    contentItem["text"] = QString("Device '%1' has been successfully registered.").arg(dev.deviceName);
    
    return QJsonObject{
        {"content", QJsonArray{contentItem}}
    };
}