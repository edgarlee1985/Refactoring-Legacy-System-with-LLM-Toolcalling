#pragma once

struct UserInfo;

class UserDialogController
{
public:
    UserDialogController();
    ~UserDialogController();

    UserInfo* getUserInfo(int idx) const;
    void setUserInfo(int idx, UserInfo* userInfo);
    void deleteUserInfo(int idx);
    void addNewUserInfo(UserInfo* userInfo);

};