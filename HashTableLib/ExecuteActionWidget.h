#pragma once

#include "Action.h"
#include <QWidget>


class QRadioButton;
class QLineEdit;
class QPushButton;


class ExecuteActionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExecuteActionWidget(QWidget *parent = nullptr);

private:
    QRadioButton *_insertButton;
    QRadioButton *_eraseButton;
    QRadioButton *_findButton;
    QLineEdit *_valueEdit;
    QPushButton *_executeButton;

    void handleExecuteButton();

signals:
    void executeAction(IntAction);
};

