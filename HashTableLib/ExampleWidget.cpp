#include "ExampleWidget.h"

#include "PresentationTabWidget.h"

#include "OperationManagerWidget.h"
#include "OperationListModel.h"
#include "HashTableDefinition.h"
#include "HashTableManagerWidget.h"
#include "HashTableFactory.h"
#include "HashTableModel.h"

#include <QHBoxLayout>


ExampleWidget::ExampleWidget(std::vector<HashTableDefinition> &&definitions,  QWidget *parent)
    : QTabWidget(parent)
    , _actionManagerWidget(nullptr)
    , _actionListModel(nullptr)
    , _hashTableManagerWidgetList()
    , _hashTableModelList()
{
    _actionManagerWidget = new OperationManagerWidget;
    _actionListModel = new OperationListModel(this);
    _actionManagerWidget->setModel(_actionListModel);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(_actionManagerWidget);

    setLayout(mainLayout);

    connect(this, &ExampleWidget::loadOperationList, _actionListModel, &OperationListModel::loadOperationListHandler);
    connect(_actionManagerWidget, &OperationManagerWidget::executeOperation, _actionListModel, &OperationListModel::executeOperationHandler);
    connect(_actionManagerWidget, &OperationManagerWidget::undoOperation, _actionListModel, &OperationListModel::undoOperationHandler);
    connect(_actionManagerWidget, &OperationManagerWidget::redoOperation, _actionListModel, &OperationListModel::redoOperationHandler);

    for (const auto &definition : definitions)
    {
        addHashTable(definition);
    }
}


void ExampleWidget::loadOperations(std::vector<IntOperation> &&operationList)
{
    qDebug() << "emit: ExampleWidget::loadOperationList()" << operationList.size();
    emit loadOperationList(operationList);
}


void ExampleWidget::addHashTable(const HashTableDefinition &definition)
{
    HashTableManagerWidget *hashTableManagerWidget = new HashTableManagerWidget(definition);
    HashTableModel *hashTableModel = new HashTableModel(HashTableFactory::Build(definition), definition);
    hashTableManagerWidget->setModel(hashTableModel);
    layout()->addWidget(hashTableManagerWidget);

    connect(this, &ExampleWidget::loadOperationList, hashTableModel, &HashTableModel::loadOperationListHandler);

    connect(hashTableModel, &HashTableModel::actionResultCalculated, hashTableManagerWidget, &HashTableManagerWidget::actionResultCalculatedHandler);

    connect(_actionManagerWidget, &OperationManagerWidget::executeOperation, hashTableModel, &HashTableModel::executeOperationHandler);
    connect(_actionManagerWidget, &OperationManagerWidget::undoOperation, hashTableModel, &HashTableModel::undoOperationHandler);
    connect(_actionManagerWidget, &OperationManagerWidget::redoOperation, hashTableModel, &HashTableModel::redoOperationHandler);

    _hashTableManagerWidgetList.push_back(hashTableManagerWidget);
    _hashTableModelList.push_back(hashTableModel);
}

