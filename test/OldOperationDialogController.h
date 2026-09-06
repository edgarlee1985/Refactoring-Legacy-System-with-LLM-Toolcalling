#pragma once
#include <QString>

class UserInfo;

class OldOperationDialogController
{
public:
    OldOperationDialogController();
    ~OldOperationDialogController();

    QString deviceCompute(const UserInfo* currentUser);
};