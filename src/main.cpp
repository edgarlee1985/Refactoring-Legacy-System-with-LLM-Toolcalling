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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QApplication::setApplicationName("QtLegacyProject");
    QApplication::setApplicationVersion("1.0");

#if defined(APPROVAL_TEST_MODE)
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