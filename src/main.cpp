#include "QtLegacyProject.h"
#include <QApplication>
#include <QDebug>
#include "test/TestDeviceOperation.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QApplication::setApplicationName("QtLegacyProject");
    QApplication::setApplicationVersion("1.0");

#ifdef TEST_MODE
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