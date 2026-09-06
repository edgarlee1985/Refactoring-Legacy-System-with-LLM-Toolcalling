#include "TestDeviceOperation.h"
#include "../src/UserDialogController.h"
#include "../src/DeviceDialogController.h"
#include "../src/OperationDialogController.h"
#include "../src/GlobalData.h"
#include "OldOperationDialogController.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QApplication>
#include <cstdlib> // 提供 srand()

enum TestResultCode {
    TEST_SUCCESS = 0,
    TEST_ERR_OUTPUT_MISMATCH = -1,
    TEST_ERR_NULL_POINTER = -2,
    TEST_ERR_FILE_MISSING = -3
};

// 測試外殼
int TestDeviceOperationCase()
{
    DeviceDialogController* deviceController = new DeviceDialogController();
    UserDialogController* userController = new UserDialogController();
    OperationDialogController* operationController = new OperationDialogController();
    OldOperationDialogController* oldOperationController = new OldOperationDialogController();

    // ==========================================
    // 讀取外部 JSON 檔案
    // ==========================================
    QString exePath = QCoreApplication::applicationDirPath();
    QString filePath = exePath + "/../test/test_data.json";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "[TEST FAILED] Cannot open test_data.json. Make sure it is in the working directory.";
        return TEST_ERR_FILE_MISSING;
    }
    
    QByteArray fileData = file.readAll();
    file.close();
    
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qCritical() << "[TEST FAILED] Invalid JSON format in test_data.json.";
        return TEST_ERR_FILE_MISSING;
    }
    QJsonObject rootObj = jsonDoc.object();

    // ==========================================
    // 從 JSON 補齊 User 測試資料
    // ==========================================
    userController->initializeUserInfo(); 

    UserInfo testUser;
    QJsonArray usersArray = rootObj["users"].toArray();
    if (!usersArray.isEmpty()) {
        QJsonObject uObj = usersArray[0].toObject();
        testUser.username = uObj["username"].toString();
        testUser.isAdmin = uObj["isAdmin"].toBool();
        testUser.canEditDevices = uObj["canEditDevices"].toBool();
        testUser.canRunOps = uObj["canRunOps"].toBool();
        
        userController->addNewUserInfo(&testUser); 
    } else {
        qCritical() << "[TEST FAILED] No user data found in JSON.";
        return TEST_ERR_FILE_MISSING;
    }

    // ==========================================
    // 從 JSON 補齊 Device 測試資料
    // ==========================================
    // 參數矩陣, 全由 LLM 生成
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

    // 執行測試前，控制隨機數種子以確保兩次執行的 mockData 完全相同
    int fixedSeed = 12345;

    // 執行舊邏輯
    srand(fixedSeed); 
    QString oldOutput = oldOperationController->deviceCompute(&testUser);

    // 執行新邏輯 (重置同一個種子，讓 rand() 產生的序列與舊邏輯一致)
    srand(fixedSeed); 
    QString newOutput = operationController->deviceCompute(&testUser);

    // 釋放記憶體
    delete deviceController;
    delete userController;
    delete operationController;
    delete oldOperationController;

    // 斷言與詳細錯誤訊息輸出
    if (newOutput != oldOutput) {
        // 使用 qCritical() 或 qWarning() 輸出紅字/警告等級的訊息
        qCritical() << "[TEST FAILED] TestDeviceOperationCase: Output mismatch!";
        qCritical() << "================= EXPECTED (Old) =================";
        qCritical().noquote() << oldOutput;
        qCritical() << "================= ACTUAL (New)   =================";
        qCritical().noquote() << newOutput;
        qCritical() << "==================================================";
        
        // 回傳定義好的錯誤碼
        return TEST_ERR_OUTPUT_MISMATCH; 
    }

    qInfo() << "[TEST PASSED] TestDeviceOperationCase completed successfully.";
    
    // 成功回傳 0
    return TEST_SUCCESS;
}