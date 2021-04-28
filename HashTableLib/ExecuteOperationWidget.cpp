#include "ExecuteOperationWidget.h"
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>


ExecuteOperationWidget::ExecuteOperationWidget(QWidget *parent)
    : QWidget(parent)
    , _insertButton(nullptr)
    , _eraseButton(nullptr)
    , _findButton(nullptr)
    , _valueEdit(nullptr)
    , _executeButton(nullptr)
{
    _findButton = new QRadioButton(tr("&FIND"));
    _insertButton = new QRadioButton(tr("&INSERT"));
    _eraseButton = new QRadioButton(tr("E&RASE"));

    _findButton->setChecked(true);

    _valueEdit = new QLineEdit;
    _valueEdit->setFocus();

    QGridLayout *selectorLayout = new QGridLayout;
    selectorLayout->addWidget(_insertButton, 0, 0);
    selectorLayout->addWidget(_findButton, 1, 0);
    selectorLayout->addWidget(_eraseButton, 2, 0);
    selectorLayout->addWidget(_valueEdit, 1, 1);

    QGroupBox *selectorBox = new QGroupBox;
    selectorBox->setLayout(selectorLayout);

    _executeButton = new QPushButton("&EXECUTE");

    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(selectorBox);
    widgetLayout->addWidget(_executeButton);

    QGroupBox *widgetBox = new QGroupBox("EXECUTE ACTION");
    widgetBox->setLayout(widgetLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(widgetBox);
    setLayout(mainLayout);

    connect(_executeButton, &QPushButton::released, this, &ExecuteOperationWidget::handleExecuteButton);
}


void ExecuteOperationWidget::handleExecuteButton()
{
    if (_valueEdit->text().isEmpty())
    {
        _valueEdit->setFocus();
        return;
    }

    bool ok = true;
    int value = _valueEdit->text().toInt(&ok);
    if (!ok)
    {
        _valueEdit->clear();
        _valueEdit->setFocus();
        return;
    }

    OperationType actionType = OperationType::INSERT;
    if (_eraseButton->isChecked())
    {
        actionType = OperationType::ERASE;
    }
    else if (_findButton->isChecked())
    {
        actionType = OperationType::FIND;
    }

    IntOperation action(actionType, value);

    emit executeOperation(action);
}
