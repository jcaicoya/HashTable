#include "MainWindow.h"
#include "OperationManagerWidget.h"
#include "OperationListModel.h"
#include "HashTableManagerWidget.h"
#include "HashTableModel.h"
#include "HashTableFactory.h"
#include "HashTableDefinition.h"
#include "RemoveHashTableDialog.h"
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _actionManagerWidget(nullptr)
    , _actionListModel(nullptr)
    , _hashTableManagerWidgetList()
    , _hashTableModelList()
{
    _actionManagerWidget = new OperationManagerWidget;
    _actionListModel = new OperationListModel(this);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(_actionManagerWidget);

    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    _actionManagerWidget->setModel(_actionListModel);

    createOperations();

    connect(this, &MainWindow::loadOperationList, _actionListModel, &OperationListModel::loadOperationListHandler);
    connect(_actionManagerWidget, &OperationManagerWidget::executeOperation, _actionListModel, &OperationListModel::executeOperationHandler);
    connect(_actionManagerWidget, &OperationManagerWidget::undoOperation, _actionListModel, &OperationListModel::undoOperationHandler);
    connect(_actionManagerWidget, &OperationManagerWidget::redoOperation, _actionListModel, &OperationListModel::redoOperationHandler);

    example1ActionSlot();
}


void MainWindow::createOperations()
{
    QMenu *examplesMenu = menuBar()->addMenu("&Examples");
    QToolBar *examplesToolBar = addToolBar("Examples");

    QAction *example1Action = new QAction(tr("&1: Matrix/Array"), this);
    example1Action->setStatusTip(tr("Matrix and array hash tables"));
    connect(example1Action, &QAction::triggered, this, &MainWindow::example1ActionSlot);
    examplesMenu->addAction(example1Action);
    examplesToolBar->addAction(example1Action);

    /*
    QMenu *configurationMenu = menuBar()->addMenu(tr("&Configurations"));
    QToolBar *configurationToolBar = addToolBar(tr("Configurations"));

    QAction *sizeConfigurationOperation = new QAction(tr("Sizes"), this);
    sizeConfigurationOperation->setStatusTip(tr("Load three tables with different sizes"));
    connect(sizeConfigurationOperation, &QAction::triggered, this, &MainWindow::loadSizeConfigurationOperationSlot);
    configurationMenu->addAction(sizeConfigurationOperation);
    configurationToolBar->addAction(sizeConfigurationOperation);

    QAction *hashConfigurationOperation = new QAction(tr("Hashes"), this);
    hashConfigurationOperation->setStatusTip(tr("Load three tables with different hashes"));
    connect(hashConfigurationOperation, &QAction::triggered, this, &MainWindow::loadHashConfigurationOperationSlot);
    configurationMenu->addAction(hashConfigurationOperation);
    configurationToolBar->addAction(hashConfigurationOperation);

    QAction *rehashConfigurationOperation = new QAction(tr("Rehashes"), this);
    hashConfigurationOperation->setStatusTip(tr("Load three tables with different rehashes"));
    connect(rehashConfigurationOperation, &QAction::triggered, this, &MainWindow::loadRehashConfigurationOperationSlot);
    configurationMenu->addAction(rehashConfigurationOperation);
    configurationToolBar->addAction(rehashConfigurationOperation);


    QMenu *operationMenu = menuBar()->addMenu(tr("&Operations"));
    QToolBar *operationToolBar = addToolBar(tr("Operations"));

    QAction *sizeOperationOperation = new QAction(tr("Sizes"), this);
    sizeOperationOperation->setStatusTip(tr("Load operation list example for different sizes"));
    connect(sizeOperationOperation, &QAction::triggered, this, &MainWindow::loadSizeOperationListOperationSlot);
    operationMenu->addAction(sizeOperationOperation);
    operationToolBar->addAction(sizeOperationOperation);

    QAction *hashOperationOperation = new QAction(tr("Hashes"), this);
    hashOperationOperation->setStatusTip(tr("Load operation example for different hashes"));
    connect(hashOperationOperation, &QAction::triggered, this, &MainWindow::loadHashOperationListOperationSlot);
    operationMenu->addAction(hashOperationOperation);
    operationToolBar->addAction(hashOperationOperation);

    QAction *rehashOperationOperation = new QAction(tr("Rehashes"), this);
    hashOperationOperation->setStatusTip(tr("Load operation example for different rehashes"));
    connect(rehashOperationOperation, &QAction::triggered, this, &MainWindow::loadRehashOperationListOperationSlot);
    operationMenu->addAction(rehashOperationOperation);
    operationToolBar->addAction(rehashOperationOperation);


    QMenu *hashTableMenu = menuBar()->addMenu(tr("&HashTables"));
    QToolBar *HashTableToolBar = addToolBar(tr("ArrayHashTable"));

    QAction *newHashTableOperation = new QAction(tr("Add"), this);
    newHashTableOperation->setStatusTip(tr("Create a new hash table"));
    connect(newHashTableOperation, &QAction::triggered, this, &MainWindow::addHashTableOperationSlot);
    hashTableMenu->addAction(newHashTableOperation);
    HashTableToolBar->addAction(newHashTableOperation);

    QAction *removeHashTableOperation = new QAction(tr("Remove"), this);
    removeHashTableOperation->setStatusTip(tr("Remove hash table"));
    connect(removeHashTableOperation, &QAction::triggered, this, &MainWindow::removeHashTableOperationSlot);
    hashTableMenu->addAction(removeHashTableOperation);
    HashTableToolBar->addAction(removeHashTableOperation);

    QAction *popHashTableOperation = new QAction(tr("Pop"), this);
    popHashTableOperation->setStatusTip(tr("Remove last hash table"));
    connect(popHashTableOperation, &QAction::triggered, this, &MainWindow::popHashTableOperationSlot);
    hashTableMenu->addAction(popHashTableOperation);
    HashTableToolBar->addAction(popHashTableOperation);

    QAction *clearHashTableOperation = new QAction(tr("Clear"), this);
    clearHashTableOperation->setStatusTip(tr("Remove all hash tables"));
    connect(clearHashTableOperation, &QAction::triggered, this, &MainWindow::clearHashTablesOperationSlot);
    hashTableMenu->addAction(clearHashTableOperation);
    HashTableToolBar->addAction(clearHashTableOperation);
    */
}


void MainWindow::addHashTable(const HashTableDefinition &definition)
{
    HashTableManagerWidget *hashTableManagerWidget = new HashTableManagerWidget(definition);
    HashTableModel *hashTableModel = new HashTableModel(HashTableFactory::Build(definition), definition);
    hashTableManagerWidget->setModel(hashTableModel);
    this->centralWidget()->layout()->addWidget(hashTableManagerWidget);

    connect(this, &MainWindow::loadOperationList, hashTableModel, &HashTableModel::loadOperationListHandler);

    connect(hashTableModel, &HashTableModel::actionResultCalculated, hashTableManagerWidget, &HashTableManagerWidget::actionResultCalculatedHandler);

    connect(_actionManagerWidget, &OperationManagerWidget::executeOperation, hashTableModel, &HashTableModel::executeOperationHandler);
    connect(_actionManagerWidget, &OperationManagerWidget::undoOperation, hashTableModel, &HashTableModel::undoOperationHandler);
    connect(_actionManagerWidget, &OperationManagerWidget::redoOperation, hashTableModel, &HashTableModel::redoOperationHandler);

    _hashTableManagerWidgetList.push_back(hashTableManagerWidget);
    _hashTableModelList.push_back(hashTableModel);
}


void MainWindow::removeHashTable(size_t tableIndex)
{
    auto hashTableManagerWidget = _hashTableManagerWidgetList.takeAt(tableIndex);
    auto hashTableModel = _hashTableModelList.takeAt(tableIndex);

    disconnect(this, &MainWindow::loadOperationList, hashTableModel, &HashTableModel::loadOperationListHandler);
    disconnect(hashTableModel, &HashTableModel::actionResultCalculated, hashTableManagerWidget, &HashTableManagerWidget::actionResultCalculatedHandler);
    disconnect(_actionManagerWidget, &OperationManagerWidget::executeOperation, hashTableModel, &HashTableModel::executeOperationHandler);
    disconnect(_actionManagerWidget, &OperationManagerWidget::undoOperation, hashTableModel, &HashTableModel::undoOperationHandler);
    disconnect(_actionManagerWidget, &OperationManagerWidget::redoOperation, hashTableModel, &HashTableModel::redoOperationHandler);

    hashTableManagerWidget->deleteLater();
    hashTableModel->deleteLater();
}


void MainWindow::popHashTable()
{
    if (!_hashTableManagerWidgetList.empty())
    {
        removeHashTable(_hashTableManagerWidgetList.size() - 1);
    }
}


void MainWindow::clearHashTables()
{
    while (!_hashTableManagerWidgetList.empty())
    {
        removeHashTable(_hashTableManagerWidgetList.size() - 1);
    }
}


void MainWindow::removeHashTableHandler(size_t tableIndex)
{
    removeHashTable(tableIndex);
}


void MainWindow::example1ActionSlot()
{
    clearHashTables();
    addHashTable({hash_table::Type::MATRIX, HashFunction::Type::Identity, RehashFunction::Type::Identity, 4});
    addHashTable({hash_table::Type::ARRAY, HashFunction::Type::Identity, RehashFunction::Type::Identity, 11});

    std::vector<IntOperation> actionList
    {
        { OperationType::INSERT, 3 },
        { OperationType::ERASE, 3 },
        { OperationType::INSERT, 13 },
        { OperationType::INSERT, 23 },
        { OperationType::INSERT, 33 },
        { OperationType::INSERT, 43 },
        { OperationType::INSERT, 53 }
    };

    emit loadOperationList(actionList);
}


/*
void MainWindow::clearHashTablesOperationSlot()
{
    while (!_hashTableManagerWidgetList.empty())
    {
        popHashTable();
    }
}



void MainWindow::popHashTableOperationSlot()
{
    popHashTable();
}


void MainWindow::loadSizeConfigurationOperationSlot()
{
    clearHashTables();
    addHashTable({hash_table::Type::MATRIX, HashFunction::Type::Identity, RehashFunction::Type::Identity, 7});
    addHashTable({hash_table::Type::MATRIX, HashFunction::Type::Identity, RehashFunction::Type::Identity, 11});
    addHashTable({hash_table::Type::MATRIX, HashFunction::Type::Identity, RehashFunction::Type::Identity, 17});
}


void MainWindow::loadHashConfigurationOperationSlot()
{
    clearHashTables();
    addHashTable({hash_table::Type::MATRIX, HashFunction::Type::Identity, RehashFunction::Type::Identity, 11});
    addHashTable({hash_table::Type::MATRIX, HashFunction::Type::Square, RehashFunction::Type::Identity, 11});
    addHashTable({hash_table::Type::MATRIX, HashFunction::Type::Cube, RehashFunction::Type::Identity, 11});
}


void MainWindow::loadRehashConfigurationOperationSlot()
{
    clearHashTables();
    addHashTable({hash_table::Type::MATRIX, HashFunction::Type::Identity, RehashFunction::Type::Identity, 11});
    addHashTable({hash_table::Type::MATRIX, HashFunction::Type::Identity, RehashFunction::Type::Square, 11});
    addHashTable({hash_table::Type::MATRIX, HashFunction::Type::Identity, RehashFunction::Type::Cube, 11});
}


void MainWindow::loadSizeOperationListOperationSlot()
{
    std::vector<IntOperation> actionList
    {
        { OperationType::INSERT, 3 },
        { OperationType::ERASE, 3 },
        { OperationType::INSERT, 13 },
        { OperationType::INSERT, 23 },
        { OperationType::INSERT, 33 },
        { OperationType::INSERT, 43 },
        { OperationType::INSERT, 53 }
    };

    emit loadOperationList(actionList);
}


void MainWindow::loadHashOperationListOperationSlot()
{
    std::vector<IntOperation> actionList
    {
        { OperationType::INSERT, 3 },
        { OperationType::INSERT, 13 },
        { OperationType::INSERT, 23 },
        { OperationType::INSERT, 33 },
        { OperationType::INSERT, 43 },
        { OperationType::INSERT, 53 }
    };
    emit loadOperationList(actionList);
}


void MainWindow::loadRehashOperationListOperationSlot()
{
    std::vector<IntOperation> actionList
    {
        { OperationType::INSERT, 3 },
        { OperationType::INSERT, 13 },
        { OperationType::INSERT, 23 },
        { OperationType::INSERT, 33 },
        { OperationType::INSERT, 43 },
        { OperationType::INSERT, 53 }
    };
    emit loadOperationList(actionList);
}


void MainWindow::addHashTableOperationSlot()
{
    // TODO
   QDialog *dialog = new QDialog;
   dialog->exec();
   static std::size_t tableSize = 10;
   addHashTable({hash_table::Type::MATRIX, HashFunction::Type::Identity , RehashFunction::Type::Identity, tableSize++});
}


void MainWindow::removeHashTableOperationSlot()
{
    RemoveHashTableDialog *removeHashTableDialog = new RemoveHashTableDialog(_hashTableModelList);
    connect(removeHashTableDialog, &RemoveHashTableDialog::removeHashTable, this, &MainWindow::removeHashTableHandler);
    removeHashTableDialog->exec();
}
*/
