#pragma once
#include "ui_QtLegacyProject.h"
#include <QMainWindow>
#include <QPushButton>

class QtLegacyProject : public QMainWindow {
    Q_OBJECT
    
public:
    QtLegacyProject(QWidget* parent = nullptr);
    ~QtLegacyProject();

private slots:
    void openDeviceDialog();
    void openUserDialog();
    void openOperationDialog();

private:
    Ui_QtLegacyProject* ui;
    
    // UI 指標
    QPushButton* btnDevice;
    QPushButton* btnUser;
    QPushButton* btnOperation;
};