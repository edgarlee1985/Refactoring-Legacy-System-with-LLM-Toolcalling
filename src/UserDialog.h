#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QListWidget>
#include "GlobalData.h"

class UserDialog : public QDialog {
    Q_OBJECT
public:
    explicit UserDialog(QWidget *parent = nullptr);

private slots:
    void onAddUser();
    void onUserSelected();
    void onUpdateUser();
    void onDeleteUser();
    void refreshUserList();

private:
    QLineEdit* txtUsername;
    QCheckBox* chkEditDevice;
    QCheckBox* chkRunOps;
    QListWidget* listUsers;
    QPushButton* btnAddUser;
    QPushButton* btnUpdateUser;
    QPushButton* btnDeleteUser;
};