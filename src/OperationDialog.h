#pragma once
#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include "GlobalData.h"

class OperationDialog : public QDialog {
    Q_OBJECT
public:
    explicit OperationDialog(QWidget *parent = nullptr);

private slots:
    void runOperations();

private:
    QComboBox* comboUser;
    QTextEdit* txtOutput;
};