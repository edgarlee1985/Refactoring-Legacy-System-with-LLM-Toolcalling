#include "QtLegacyProject.h"
#include <QVBoxLayout>
#include "DeviceDialog.h"
#include "UserDialog.h"
#include "OperationDialog.h"

QtLegacyProject::QtLegacyProject(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_QtLegacyProject)
{
    ui->setupUi(this);

    // 在你原本空白的 centralwidget 中加入按鈕
    QVBoxLayout* mainLayout = new QVBoxLayout(ui->centralwidget);

    btnDevice = new QPushButton("1. Configure Devices", this);
    btnUser = new QPushButton("2. Configure Users", this);
    btnOperation = new QPushButton("3. Run Operations", this);

    mainLayout->addWidget(btnDevice);
    mainLayout->addWidget(btnUser);
    mainLayout->addWidget(btnOperation);

    connect(btnDevice, &QPushButton::clicked, this, &QtLegacyProject::openDeviceDialog);
    connect(btnUser, &QPushButton::clicked, this, &QtLegacyProject::openUserDialog);
    connect(btnOperation, &QPushButton::clicked, this, &QtLegacyProject::openOperationDialog);
}

QtLegacyProject::~QtLegacyProject()
{
    delete ui; 
}

void QtLegacyProject::openDeviceDialog() {
    DeviceDialog dlg(this);
    dlg.exec();
}

void QtLegacyProject::openUserDialog() {
    UserDialog dlg(this);
    dlg.exec();
}

void QtLegacyProject::openOperationDialog() {
    OperationDialog dlg(this);
    dlg.exec();
}