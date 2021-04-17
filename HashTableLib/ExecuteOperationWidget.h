#pragma once

#include "Operation.h"
#include <QWidget>


class QRadioButton;
class QLineEdit;
class QPushButton;


class ExecuteOperationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExecuteOperationWidget(QWidget *parent = nullptr);

private:
    QRadioButton *_insertButton;
    QRadioButton *_eraseButton;
    QRadioButton *_findButton;
    QLineEdit *_valueEdit;
    QPushButton *_executeButton;

    void handleExecuteButton();

signals:
    void executeOperation(IntOperation);
};

