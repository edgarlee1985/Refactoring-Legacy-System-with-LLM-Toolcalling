#include "UserDialog.h"
#include "UserDialogController.h"

UserDialog::UserDialog(QWidget *parent)
: QDialog(parent)
{
    userController = new UserDialogController();
    
    setWindowTitle("User Permissions");
    QVBoxLayout* layout = new QVBoxLayout(this);

    // 初始化預設 Admin (如果還沒初始化過)
    if (g_userCount == 0) {
        g_users[0] = {"Admin", true, true, true};
        g_userCount++;
    }

    listUsers = new QListWidget(this);

    txtUsername = new QLineEdit(this);
    txtUsername->setPlaceholderText("New Username");
    chkEditDevice = new QCheckBox("Can Edit Devices", this);
    chkRunOps = new QCheckBox("Can Run Operations", this);

    btnAddUser = new QPushButton("Add User", this);
    btnUpdateUser = new QPushButton("Update Selected User", this);
    btnUpdateUser->setEnabled(false);

    btnDeleteUser = new QPushButton("Delete Selected User", this);
    btnDeleteUser->setEnabled(false);

    layout->addWidget(listUsers);
    layout->addWidget(txtUsername);
    layout->addWidget(chkEditDevice);
    layout->addWidget(chkRunOps);
    layout->addWidget(btnAddUser);
    layout->addWidget(btnUpdateUser);
    layout->addWidget(btnDeleteUser);

    connect(btnAddUser, &QPushButton::clicked, this, &UserDialog::onAddUser);
    connect(listUsers, &QListWidget::itemSelectionChanged, this, &UserDialog::onUserSelected);
    connect(btnUpdateUser, &QPushButton::clicked, this, &UserDialog::onUpdateUser);
    connect(btnDeleteUser, &QPushButton::clicked, this, &UserDialog::onDeleteUser);

    // 視窗建立時，先載入一次目前的列表
    refreshUserList();
}

void UserDialog::onUserSelected()
{
    int idx = listUsers->currentRow();
    if (idx < 0 || idx >= g_userCount) {
        btnUpdateUser->setEnabled(false);
        btnDeleteUser->setEnabled(false);
        return;
    }

    // 啟動更新按鈕
    btnUpdateUser->setEnabled(true);

    if (idx == 0) {
        btnDeleteUser->setEnabled(false);
    } else {
        btnDeleteUser->setEnabled(true);
    }

    UserInfo* userInfo = userController->getUserInfo(idx);
    // 將資料倒回 UI
    txtUsername->setText(userInfo->username);
    chkEditDevice->setChecked(userInfo->canEditDevices);
    chkRunOps->setChecked(userInfo->canRunOps);
}

void UserDialog::onUpdateUser()
{
    int idx = listUsers->currentRow();
    if (idx < 0 || idx >= g_userCount) return;

    UserInfo* userInfo = new UserInfo;

    userInfo->username = txtUsername->text();
    // 預設不允許透過 UI 更改 Admin 核心權限，這裡維持原本的 isAdmin 狀態
    userInfo->canEditDevices = chkEditDevice->isChecked();
    userInfo->canRunOps = chkRunOps->isChecked();

    userController->setUserInfo(idx, userInfo);

    QMessageBox::information(this, "Updated", "User data updated successfully.");
    
    refreshUserList();
    
    // 重新選中剛剛更新的項目
    listUsers->setCurrentRow(idx);
}

void UserDialog::onDeleteUser()
{
    int idx = listUsers->currentRow();
    
    // 再次檢查，確保不會誤刪 Admin 或越界
    if (idx <= 0 || idx >= g_userCount) return;

    userController->deleteUserInfo(idx);
    
    QMessageBox::information(this, "Deleted", "User deleted successfully.");
    
    refreshUserList();
    
    // 刪除後清空畫面狀態與按鈕
    txtUsername->clear();
    chkEditDevice->setChecked(false);
    chkRunOps->setChecked(false);
    btnUpdateUser->setEnabled(false);
    btnDeleteUser->setEnabled(false);
}

void UserDialog::onAddUser()
{
    if (g_userCount >= MAX_USERS) {
        QMessageBox::warning(this, "Error", "User Array is FULL!");
        return; 
    }

    UserInfo* newUserInfo = new UserInfo;
    newUserInfo->username = txtUsername->text();
    newUserInfo->isAdmin = false;
    newUserInfo->canEditDevices = chkEditDevice->isChecked();
    newUserInfo->canRunOps = chkRunOps->isChecked();

    userController->addNewUserInfo(newUserInfo);

    QMessageBox::information(this, "Success", "User added to g_users[]");
    
    // 重新整理 UI 列表
    refreshUserList();

    // 清空輸入框狀態，方便連續輸入下一個 User
    txtUsername->clear();
    chkEditDevice->setChecked(false);
    chkRunOps->setChecked(false);
}

void UserDialog::refreshUserList()
{
    listUsers->clear();
    
    for (int i = 0; i < g_userCount; ++i) {
        QString userInfo = g_users[i].username;
        
        // 判斷權限並附加到顯示字串後方
        if (g_users[i].isAdmin) {
            userInfo += " [Admin]";
        } else {
            userInfo += " (Edit: " + QString(g_users[i].canEditDevices ? "Y" : "N") + 
                        ", Run: " + QString(g_users[i].canRunOps ? "Y" : "N") + ")";
        }
        
        listUsers->addItem(userInfo);
    }
}