#include "OperationDialog.h"
#include "OperationDialogController.h"

OperationDialog::OperationDialog(QWidget *parent)
: QDialog(parent)
{
    operationController = new OperationDialogController();

    setWindowTitle("Operations Run (Spaghetti Logic)");
    QVBoxLayout* layout = new QVBoxLayout(this);

    comboUser = new QComboBox(this);
    for(int i = 0; i < g_userCount; i++) {
        comboUser->addItem(g_users[i].username);
    }

    QPushButton* btnRun = new QPushButton("Execute Legacy Routine", this);
    txtOutput = new QTextEdit(this);
    txtOutput->setReadOnly(true);

    layout->addWidget(comboUser);
    layout->addWidget(btnRun);
    layout->addWidget(txtOutput);

    connect(btnRun, &QPushButton::clicked, this, &OperationDialog::runOperations);
}

void OperationDialog::runOperations()
{
    txtOutput->clear();
    int userIdx = comboUser->currentIndex();
    if (userIdx < 0 || userIdx >= g_userCount) return;

    UserInfo currentUser = g_users[userIdx];

    // 權限檢查
    if (!currentUser.isAdmin && !currentUser.canRunOps) {
        txtOutput->append("[Error] " + currentUser.username + " lacks permission to run operations.");
        return;
    }

    txtOutput->append("Starting operations as: " + currentUser.username + "...\n");

    // 巡覽全域陣列，執行不同 Device 的混亂邏輯
    QString text = operationController->deviceCompute(&currentUser);
    txtOutput->append(text);
    txtOutput->append("\nOperation routine finished.");
}