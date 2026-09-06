#include "UserDialogController.h"
#include "GlobalData.h"

UserDialogController::UserDialogController()
{

}

UserDialogController::~UserDialogController()
{
    
}

void UserDialogController::initializeUserInfo()
{
    // 初始化預設 Admin (如果還沒初始化過)
    if (g_userCount == 0) {
        g_users[0] = {"Admin", true, true, true};
        g_userCount++;
    }
}

int UserDialogController::getUserInfoCount() const
{
    return g_userCount;
}

UserInfo* UserDialogController::getUserInfo(int idx) const
{
    return &g_users[idx];
}

void UserDialogController::setUserInfo(int idx, UserInfo* userInfo)
{
    if (idx < 0 || idx >= g_userCount) return;

    g_users[idx].username = userInfo->username;
    g_users[idx].canEditDevices = userInfo->canEditDevices;
    g_users[idx].canRunOps = userInfo->canRunOps;
}

void UserDialogController::deleteUserInfo(int idx)
{
    if (idx <= 0 || idx >= g_userCount) return;

    for (int i = idx; i < g_userCount - 1; ++i) {
        g_users[i] = g_users[i + 1];
    }
    
    // 總數減 1
    g_userCount--;
}

void UserDialogController::addNewUserInfo(UserInfo* userInfo)
{
    g_users[g_userCount].username = userInfo->username;
    g_users[g_userCount].isAdmin = false;
    g_users[g_userCount].canEditDevices = userInfo->canEditDevices;
    g_users[g_userCount].canRunOps = userInfo->canRunOps;
    
    g_userCount++;
}