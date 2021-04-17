#include "OperationManagerWidget.h"

#include "ExecuteOperationWidget.h"
#include "OperationListWidget.h"
#include "OperationListModel.h"
#include <QVBoxLayout>


OperationManagerWidget::OperationManagerWidget(QWidget *parent)
    : QWidget(parent)
    , _executeOperationWidget(nullptr)
    , _actionListWidget(nullptr)
{
    _executeOperationWidget = new ExecuteOperationWidget(this);
    _actionListWidget = new OperationListWidget(this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(_executeOperationWidget);
    layout->addWidget(_actionListWidget);

    this->setLayout(layout);

    connect(_executeOperationWidget, &ExecuteOperationWidget::executeOperation, this, &OperationManagerWidget::executeOperationHandler);
    connect(_actionListWidget, &OperationListWidget::undoOperation, this, &OperationManagerWidget::undoOperationHandler);
    connect(_actionListWidget, &OperationListWidget::redoOperation, this, &OperationManagerWidget::redoOperationHandler);
}


void OperationManagerWidget::executeOperationHandler(IntOperation action)
{
    emit executeOperation(action);
}


void OperationManagerWidget::undoOperationHandler()
{
    emit undoOperation();
}


void OperationManagerWidget::redoOperationHandler()
{
    emit redoOperation();
}


void OperationManagerWidget::setModel(OperationListModel *actionListModel)
{
    _actionListWidget->setModel(actionListModel);
    connect(actionListModel, &OperationListModel::currentPositionChanged, _actionListWidget, &OperationListWidget::currentPositionChangedHandler);
}
