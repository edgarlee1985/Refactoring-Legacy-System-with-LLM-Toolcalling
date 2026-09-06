#include "TestDeviceOperation.h"
#include "../src/UserDialogController.h"
#include "../src/DeviceDialogController.h"
#include "../src/OperationDialogController.h"
#include "../src/GlobalData.h"
#include "OldOperationDialogController.h"

#include <QDebug>
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
    // 1. 補齊 User 測試資料
    // ==========================================
    // 初始化預設 Admin 帳號
    userController->initializeUserInfo(); 

    // 隨機新增一名一般使用者
    UserInfo testUser;
    testUser.username = "Operator_A";
    testUser.isAdmin = false;
    testUser.canEditDevices = true;
    testUser.canRunOps = true;
    userController->addNewUserInfo(&testUser); 

    // ==========================================
    // 2. 補齊 Device 測試資料
    // ==========================================
    
    // 參數矩陣, 全由 LLM 生成
    DeviceConfig devSensor;
    devSensor.deviceName = "TempSensor_01";
    devSensor.deviceType = 0; 
    devSensor.isActive = true;
    devSensor.isCalibrated = true;
    devSensor.thresholdValue = 40; 
    devSensor.hasAutoMode = false;
    deviceController->addNewDevice(&devSensor); 

    DeviceConfig devActuator;
    devActuator.deviceName = "MainValve_01";
    devActuator.deviceType = 1; 
    devActuator.isActive = true;
    devActuator.isCalibrated = true;
    devActuator.thresholdValue = 75; 
    devActuator.hasAutoMode = true; 
    deviceController->addNewDevice(&devActuator);

    DeviceConfig devRelay;
    devRelay.deviceName = "PowerRelay_01";
    devRelay.deviceType = 2; 
    devRelay.isActive = true;
    devRelay.isCalibrated = true;
    devRelay.thresholdValue = 0;
    devRelay.hasAutoMode = false;
    deviceController->addNewDevice(&devRelay); 

    DeviceConfig devInactive;
    devInactive.deviceName = "OldSensor_Broken";
    devInactive.deviceType = 0;
    devInactive.isActive = false; 
    devInactive.isCalibrated = false;
    devInactive.thresholdValue = 0;
    devInactive.hasAutoMode = false;
    deviceController->addNewDevice(&devInactive);

    // 3. 執行測試前，控制隨機數種子以確保兩次執行的 mockData 完全相同
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

    // 4. 斷言與詳細錯誤訊息輸出
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