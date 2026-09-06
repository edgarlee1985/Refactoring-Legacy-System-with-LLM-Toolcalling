#pragma once
#include <QString>

class UserInfo;

class OperationDialogController
{
public:
    OperationDialogController();
    ~OperationDialogController();

    QString deviceCompute(const UserInfo* currentUser);
};