#ifdef APPROVAL_TEST_MODE

#include <gtest/gtest.h>
#include "ApprovalTests.hpp"

#include "../src/UserDialogController.h"
#include "../src/DeviceDialogController.h"
#include "../src/OperationDialogController.h"
#include "../src/GlobalData.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include <cstdlib> 

using namespace ApprovalTests;

TEST(OperationControllerTest, ApprovalTestForDeviceCompute)
{
    DeviceDialogController* deviceController = new DeviceDialogController();
    UserDialogController* userController = new UserDialogController();
    OperationDialogController* operationController = new OperationDialogController();

    // 1. 讀取與設定 JSON 資料
    QString exePath = QCoreApplication::applicationDirPath();
    QString filePath = exePath + "/../test/test_data.json";
    QFile file(filePath);
    
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text)); 
    
    QByteArray fileData = file.readAll();
    file.close();
    
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData);
    ASSERT_TRUE(!jsonDoc.isNull());
    QJsonObject rootObj = jsonDoc.object();

    userController->initializeUserInfo(); 
    UserInfo testUser;
    QJsonArray usersArray = rootObj["users"].toArray();
    ASSERT_TRUE(!usersArray.isEmpty());
    
    QJsonObject uObj = usersArray[0].toObject();
    testUser.username = uObj["username"].toString();
    testUser.isAdmin = uObj["isAdmin"].toBool();
    testUser.canEditDevices = uObj["canEditDevices"].toBool();
    testUser.canRunOps = uObj["canRunOps"].toBool();
    userController->addNewUserInfo(&testUser); 

    QJsonArray devicesArray = rootObj["devices"].toArray();
    for (int i = 0; i < devicesArray.size(); ++i) {
        QJsonObject dObj = devicesArray[i].toObject();
        DeviceConfig dev;
        dev.deviceName = dObj["deviceName"].toString();
        dev.deviceType = dObj["deviceType"].toInt();
        dev.isActive = dObj["isActive"].toBool();
        dev.isCalibrated = dObj["isCalibrated"].toBool();
        dev.thresholdValue = dObj["thresholdValue"].toInt();
        dev.hasAutoMode = dObj["hasAutoMode"].toBool();
        deviceController->addNewDevice(&dev);
    }

    // 2. 執行新版業務邏輯
    int fixedSeed = 12345;
    srand(fixedSeed); 
    QString newOutput = operationController->deviceCompute(&testUser);

    // 3. 清理記憶體
    delete deviceController;
    delete userController;
    delete operationController;

    // 4. 使用 Approval Tests 進行驗證
    Approvals::verify(newOutput.toStdString());
}

#endif // APPROVAL_TEST_MODE