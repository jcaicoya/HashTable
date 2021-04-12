#pragma once

#include <QWidget>
#include "ActionResult.h"

class QLabel;
class QLineEdit;

class ActionResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ActionResultWidget(QWidget *parent = nullptr);

public slots:
    void actionResultCalculatedHandler(IntActionResult actionResult);

private:
    QLabel *_actionLabel;
    QLineEdit *_actionInfo;
    QLabel *_resultLabel;
    QLineEdit *_resultInfo;
    QLabel *_stepLabel;
    QLineEdit *_stepInfo;
};
