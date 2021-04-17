#include "OperationResultWidget.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>


OperationResultWidget::OperationResultWidget(QWidget *parent)
    : QWidget(parent)
    , _actionLabel(nullptr)
    , _actionInfo(nullptr)
    , _resultLabel(nullptr)
    , _resultInfo(nullptr)
    , _stepLabel(nullptr)
    , _stepInfo(nullptr)
{
    _actionLabel = new QLabel("ACTION:");
    _actionInfo = new QLineEdit;
    _actionLabel->setBuddy(_actionInfo);
    _actionInfo->setReadOnly(true);

    _resultLabel = new QLabel("RESULT:");
    _resultInfo = new QLineEdit;
    _resultLabel->setBuddy(_actionInfo);
    _resultInfo->setReadOnly(true);

    _stepLabel = new QLabel("STEPS:");
    _stepInfo = new QLineEdit;
    _stepLabel->setBuddy(_actionInfo);
    _stepInfo->setReadOnly(true);

    QGridLayout *widgetLayout = new QGridLayout;
    widgetLayout->addWidget(_actionLabel, 0, 0);
    widgetLayout->addWidget(_actionInfo, 0, 1);
    widgetLayout->addWidget(_resultLabel, 1, 0);
    widgetLayout->addWidget(_resultInfo, 1, 1);
    widgetLayout->addWidget(_stepLabel, 2, 0);
    widgetLayout->addWidget(_stepInfo, 2, 1);

    QGroupBox *widgetBox = new QGroupBox("ACTION RESULT");
    widgetBox->setLayout(widgetLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(widgetBox);
    setLayout(mainLayout);
}


void OperationResultWidget::actionResultCalculatedHandler(IntOperationResult actionResult)
{
    if (OperationType::NONE == actionResult.getOperation().getType())
    {
        _actionInfo->clear();
        _resultInfo->clear();
        _stepInfo->clear();
        return;
    }

    _actionInfo->setText(actionResult.getOperation().toString());
    _resultInfo->setText(array_hash_table::toString(actionResult.getResultInfo()._resultType).data());

    QString steps;
    const auto &positions = actionResult.getResultInfo()._positions;

    steps = QString::number(positions.front());
    for (auto itr = ++(positions.cbegin()); itr != positions.cend(); itr++)
    {
        steps += ", ";
        steps += QString::number(*itr);
    }

    _stepInfo->setText(steps);
}
