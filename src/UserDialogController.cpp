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