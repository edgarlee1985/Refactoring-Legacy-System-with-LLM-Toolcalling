#include "UserDialogController.h"
#include "GlobalData.h"

UserDialogController::UserDialogController()
{

}

UserDialogController::~UserDialogController()
{
    
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