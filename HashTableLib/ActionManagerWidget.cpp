#include "ActionManagerWidget.h"

#include "ExecuteActionWidget.h"
#include "ActionListWidget.h"
#include "ActionListModel.h"
#include <QVBoxLayout>


ActionManagerWidget::ActionManagerWidget(QWidget *parent)
    : QWidget(parent)
    , _executeActionWidget(nullptr)
    , _actionListWidget(nullptr)
{
    _executeActionWidget = new ExecuteActionWidget(this);
    _actionListWidget = new ActionListWidget(this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(_executeActionWidget);
    layout->addWidget(_actionListWidget);

    this->setLayout(layout);

    connect(_executeActionWidget, &ExecuteActionWidget::executeAction, this, &ActionManagerWidget::executeActionHandler);
    connect(_actionListWidget, &ActionListWidget::undoAction, this, &ActionManagerWidget::undoActionHandler);
    connect(_actionListWidget, &ActionListWidget::redoAction, this, &ActionManagerWidget::redoActionHandler);
}


void ActionManagerWidget::executeActionHandler(IntAction action)
{
    emit executeAction(action);
}


void ActionManagerWidget::undoActionHandler()
{
    qDebug() << "ActionManagerWidget::undoActionHandler()";
    emit undoAction();
}


void ActionManagerWidget::redoActionHandler()
{
    qDebug() << "ActionManagerWidget::redoActionHandler()";
    emit redoAction();
}


void ActionManagerWidget::setModel(ActionListModel *actionListModel)
{
    _actionListWidget->setModel(actionListModel);
    connect(actionListModel, &ActionListModel::currentPositionChanged, _actionListWidget, &ActionListWidget::currentPositionChangedHandler);
}
