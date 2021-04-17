#pragma once

#include <QWidget>
#include "OperationResult.h"

class QLabel;
class QLineEdit;

class OperationResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OperationResultWidget(QWidget *parent = nullptr);

public slots:
    void actionResultCalculatedHandler(IntOperationResult actionResult);

private:
    QLabel *_actionLabel;
    QLineEdit *_actionInfo;
    QLabel *_resultLabel;
    QLineEdit *_resultInfo;
    QLabel *_stepLabel;
    QLineEdit *_stepInfo;
};
