#include "QtLegacyProject.h"
#include <QApplication>
#include <QDebug>
#include "test/TestDeviceOperation.h"

#ifdef APPROVAL_TEST_MODE
#include <gtest/gtest.h>
// 告訴框架：我們有自己的 main()，只要提供實作就好
#define APPROVALS_GOOGLETEST_EXISTING_MAIN 
#include "ApprovalTests.hpp"
#endif

#ifdef LLM_MCP_MODE
#include <QJsonDocument>
#include <QJsonArray>
#include <QTextStream>
#include <cstdio>

#include "src/UserDialogController.h"
#include "src/DeviceDialogController.h"
#include "src/OperationDialogController.h"
#include "src/GlobalData.h"

#include "MCPToolStrategy/McpToolStrategy.h"
#include "MCPToolStrategy/AddDeviceStrategy.h"
#include "MCPToolStrategy/AddUserInfoStrategy.h"
#include "MCPToolStrategy/DeviceComputeStrategy.h"
#include "MCPToolStrategy/ListUsersStrategy.h"
#include "MCPToolStrategy/TestDeviceOperationStrategy.h"
#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QApplication::setApplicationName("QtLegacyProject");
    QApplication::setApplicationVersion("1.0");

#if defined(LLM_MCP_MODE)
    // 將所有 Qt 系統日誌重導向到 stderr，避免干擾 stdout 的 JSON-RPC 回應
    qInstallMessageHandler([](QtMsgType, const QMessageLogContext&, const QString& msg){
        // 改為 toUtf8() 確保跨平台編碼一致
        fprintf(stderr, "%s\n", msg.toUtf8().constData()); 
    });
    
    qInfo() << "[Mode] Starting LLM Test Mode (MCP Server via stdio)...";


    // 建立應用程式的 Stateful Controllers (生命週期隨 MCP Server 常駐)
    UserDialogController* userController = new UserDialogController();
    DeviceDialogController* deviceController = new DeviceDialogController();
    OperationDialogController* opController = new OperationDialogController();
    
    // 建立 <function name, Strategy> 的 map
    QMap<QString, McpToolStrategy*> toolStrategies;
    
    // 將實作好的 Strategy 註冊進 Map 中，未來新增功能只需在這裡註冊
    McpToolStrategy* testDeviceStrategy = new TestDeviceOperationStrategy();
    toolStrategies.insert(testDeviceStrategy->getName(), testDeviceStrategy);

    // 註冊有狀態 Tool，將 Controllers 指標注入
    McpToolStrategy* addUserStrategy = new AddUserInfoStrategy(userController);
    toolStrategies.insert(addUserStrategy->getName(), addUserStrategy);

    McpToolStrategy* addDeviceStrategy = new AddDeviceStrategy(deviceController);
    toolStrategies.insert(addDeviceStrategy->getName(), addDeviceStrategy);

    McpToolStrategy* listUsersStrategy = new ListUsersStrategy(userController);
    toolStrategies.insert(listUsersStrategy->getName(), listUsersStrategy);

    McpToolStrategy* deviceComputeStrategy = new DeviceComputeStrategy(opController, userController);
    toolStrategies.insert(deviceComputeStrategy->getName(), deviceComputeStrategy);

    QTextStream input(stdin);
    input.setCodec("UTF-8");

    QTextStream output(stdout);
    output.setCodec("UTF-8");

    // 啟動 JSON-RPC 監聽迴圈
    while (!input.atEnd()) {
        QString line = input.readLine();
        if (line.trimmed().isEmpty()) continue;

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8(), &error);
        if (error.error != QJsonParseError::NoError) continue;

        QJsonObject req = doc.object();
        QString method = req["method"].toString();
        QJsonValue id = req["id"];
        
        QJsonObject response;
        response["jsonrpc"] = "2.0";
        if (!id.isUndefined()) response["id"] = id;

        if (method == "initialize") {
            // 回應 initialize 請求，告知客戶端本伺服器支援的功能
            QJsonObject capabilities;
            capabilities["tools"] = QJsonObject(); // 宣告支援 tools 功能
            
            QJsonObject serverInfo;
            serverInfo["name"] = "QtLegacyProject_MCP";
            serverInfo["version"] = "1.0.0";
            
            response["result"] = QJsonObject{
                {"protocolVersion", "2024-11-05"}, // 需符合 MCP 協定版本
                {"capabilities", capabilities},
                {"serverInfo", serverInfo}
            };
        }
        else if (method == "tools/list") {
            QJsonArray toolsArray;
            // 動態遍歷 Map，自動收集所有註冊的 Function Schema
            for (auto strategy : toolStrategies) {
                QJsonObject tool;
                tool["name"] = strategy->getName();
                tool["description"] = strategy->getDescription();
                tool["inputSchema"] = strategy->getInputSchema();
                toolsArray.append(tool);
            }
            response["result"] = QJsonObject{
                {"tools", toolsArray}
            };
        } 
        else if (method == "tools/call") {
            QJsonObject params = req["params"].toObject();
            QString name = params["name"].toString();
            QJsonObject arguments = params["arguments"].toObject();
            
            // 透過 function name 查找對應的 Strategy 並執行
            if (toolStrategies.contains(name)) {
                response["result"] = toolStrategies[name]->execute(arguments);
            } else {
                response["error"] = QJsonObject{{"code", -32601}, {"message", "Tool not found"}};
            }
        } else {
            response["error"] = QJsonObject{{"code", -32601}, {"message", "Method not found"}};
        }

        // 將 JSON 回應輸出回 LLM
        QJsonDocument resDoc(response);
        output << resDoc.toJson(QJsonDocument::Compact) << "\n";
        output.flush();
    }
    
    // 清理資源
    qDeleteAll(toolStrategies);
    toolStrategies.clear();

    return 0;
#elif defined(APPROVAL_TEST_MODE)
    // 編譯器在 build ${PROJECT_NAME}_Approval_Test 時只會保留這裡
    qDebug() << "[Mode] Starting Approval Test Mode (Google Test)...";
    
    ::testing::InitGoogleTest(&argc, argv);

    ApprovalTests::initializeApprovalTestsForGoogleTests();
    
    return RUN_ALL_TESTS(); // 自動執行所有以 TEST() 註冊的測試案例

#elif defined(TEST_MODE)
    // 編譯器在 build ${PROJECT_NAME}_Test 時只會保留這裡
    qDebug() << "[Mode] Starting Test Mode (Console)...";
    
    int result = TestDeviceOperationCase();
    return result;
#else
    // 編譯器在 build ${PROJECT_NAME} 時只會保留這裡
    qDebug() << "[Mode] Starting Product Mode (GUI)...";
    
    QtLegacyProject w;
    w.show();
    return a.exec();
#endif
}