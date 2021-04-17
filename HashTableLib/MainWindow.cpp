#include "MainWindow.h"
#include "ActionManagerWidget.h"
#include "ActionListModel.h"
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
    _actionManagerWidget = new ActionManagerWidget;
    _actionListModel = new ActionListModel(this);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(_actionManagerWidget);

    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    _actionManagerWidget->setModel(_actionListModel);

    createActions();

    connect(this, &MainWindow::loadActionList, _actionListModel, &ActionListModel::loadActionListHandler);
    connect(_actionManagerWidget, &ActionManagerWidget::executeAction, _actionListModel, &ActionListModel::executeActionHandler);
    connect(_actionManagerWidget, &ActionManagerWidget::undoAction, _actionListModel, &ActionListModel::undoActionHandler);
    connect(_actionManagerWidget, &ActionManagerWidget::redoAction, _actionListModel, &ActionListModel::redoActionHandler);
}


void MainWindow::createActions()
{
    QMenu *configurationMenu = menuBar()->addMenu(tr("&Configurations"));
    QToolBar *configurationToolBar = addToolBar(tr("Configurations"));

    QAction *sizeConfigurationAction = new QAction(tr("Sizes"), this);
    sizeConfigurationAction->setStatusTip(tr("Load three tables with different sizes"));
    connect(sizeConfigurationAction, &QAction::triggered, this, &MainWindow::loadSizeConfigurationActionSlot);
    configurationMenu->addAction(sizeConfigurationAction);
    configurationToolBar->addAction(sizeConfigurationAction);

    QAction *hashConfigurationAction = new QAction(tr("Hashes"), this);
    hashConfigurationAction->setStatusTip(tr("Load three tables with different hashes"));
    connect(hashConfigurationAction, &QAction::triggered, this, &MainWindow::loadHashConfigurationActionSlot);
    configurationMenu->addAction(hashConfigurationAction);
    configurationToolBar->addAction(hashConfigurationAction);

    QAction *rehashConfigurationAction = new QAction(tr("Rehashes"), this);
    hashConfigurationAction->setStatusTip(tr("Load three tables with different rehashes"));
    connect(rehashConfigurationAction, &QAction::triggered, this, &MainWindow::loadRehashConfigurationActionSlot);
    configurationMenu->addAction(rehashConfigurationAction);
    configurationToolBar->addAction(rehashConfigurationAction);


    QMenu *operationMenu = menuBar()->addMenu(tr("&Operations"));
    QToolBar *operationToolBar = addToolBar(tr("Operations"));

    QAction *sizeOperationAction = new QAction(tr("Sizes"), this);
    sizeOperationAction->setStatusTip(tr("Load operation list example for different sizes"));
    connect(sizeOperationAction, &QAction::triggered, this, &MainWindow::loadSizeOperationListActionSlot);
    operationMenu->addAction(sizeOperationAction);
    operationToolBar->addAction(sizeOperationAction);

    QAction *hashOperationAction = new QAction(tr("Hashes"), this);
    hashOperationAction->setStatusTip(tr("Load operation example for different hashes"));
    connect(hashOperationAction, &QAction::triggered, this, &MainWindow::loadHashOperationListActionSlot);
    operationMenu->addAction(hashOperationAction);
    operationToolBar->addAction(hashOperationAction);

    QAction *rehashOperationAction = new QAction(tr("Rehashes"), this);
    hashOperationAction->setStatusTip(tr("Load operation example for different rehashes"));
    connect(rehashOperationAction, &QAction::triggered, this, &MainWindow::loadRehashOperationListActionSlot);
    operationMenu->addAction(rehashOperationAction);
    operationToolBar->addAction(rehashOperationAction);


    QMenu *hashTableMenu = menuBar()->addMenu(tr("&HashTables"));
    QToolBar *HashTableToolBar = addToolBar(tr("HashTable"));

    QAction *newHashTableAction = new QAction(tr("Add"), this);
    newHashTableAction->setStatusTip(tr("Create a new hash table"));
    connect(newHashTableAction, &QAction::triggered, this, &MainWindow::addHashTableActionSlot);
    hashTableMenu->addAction(newHashTableAction);
    HashTableToolBar->addAction(newHashTableAction);

    QAction *removeHashTableAction = new QAction(tr("Remove"), this);
    removeHashTableAction->setStatusTip(tr("Remove hash table"));
    connect(removeHashTableAction, &QAction::triggered, this, &MainWindow::removeHashTableActionSlot);
    hashTableMenu->addAction(removeHashTableAction);
    HashTableToolBar->addAction(removeHashTableAction);

    QAction *popHashTableAction = new QAction(tr("Pop"), this);
    popHashTableAction->setStatusTip(tr("Remove last hash table"));
    connect(popHashTableAction, &QAction::triggered, this, &MainWindow::popHashTableActionSlot);
    hashTableMenu->addAction(popHashTableAction);
    HashTableToolBar->addAction(popHashTableAction);

    QAction *clearHashTableAction = new QAction(tr("Clear"), this);
    clearHashTableAction->setStatusTip(tr("Remove all hash tables"));
    connect(clearHashTableAction, &QAction::triggered, this, &MainWindow::clearHashTablesActionSlot);
    hashTableMenu->addAction(clearHashTableAction);
    HashTableToolBar->addAction(clearHashTableAction);
}


void MainWindow::addHashTable(const HashTableDefinition &definition)
{
    HashTableManagerWidget *hashTableManagerWidget = new HashTableManagerWidget(definition);
    HashTableModel *hashTableModel = new HashTableModel(HashTableFactory::Build(definition), definition);
    hashTableManagerWidget->setModel(hashTableModel);
    this->centralWidget()->layout()->addWidget(hashTableManagerWidget);

    connect(this, &MainWindow::loadActionList, hashTableModel, &HashTableModel::loadActionListHandler);

    connect(hashTableModel, &HashTableModel::actionResultCalculated, hashTableManagerWidget, &HashTableManagerWidget::actionResultCalculatedHandler);

    connect(_actionManagerWidget, &ActionManagerWidget::executeAction, hashTableModel, &HashTableModel::executeActionHandler);
    connect(_actionManagerWidget, &ActionManagerWidget::undoAction, hashTableModel, &HashTableModel::undoActionHandler);
    connect(_actionManagerWidget, &ActionManagerWidget::redoAction, hashTableModel, &HashTableModel::redoActionHandler);

    _hashTableManagerWidgetList.push_back(hashTableManagerWidget);
    _hashTableModelList.push_back(hashTableModel);
}


void MainWindow::removeHashTable(size_t tableIndex)
{
    auto hashTableManagerWidget = _hashTableManagerWidgetList.takeAt(tableIndex);
    auto hashTableModel = _hashTableModelList.takeAt(tableIndex);

    disconnect(this, &MainWindow::loadActionList, hashTableModel, &HashTableModel::loadActionListHandler);
    disconnect(hashTableModel, &HashTableModel::actionResultCalculated, hashTableManagerWidget, &HashTableManagerWidget::actionResultCalculatedHandler);
    disconnect(_actionManagerWidget, &ActionManagerWidget::executeAction, hashTableModel, &HashTableModel::executeActionHandler);
    disconnect(_actionManagerWidget, &ActionManagerWidget::undoAction, hashTableModel, &HashTableModel::undoActionHandler);
    disconnect(_actionManagerWidget, &ActionManagerWidget::redoAction, hashTableModel, &HashTableModel::redoActionHandler);

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


void MainWindow::clearHashTablesActionSlot()
{
    while (!_hashTableManagerWidgetList.empty())
    {
        popHashTable();
    }
}



void MainWindow::popHashTableActionSlot()
{
    popHashTable();
}


void MainWindow::loadSizeConfigurationActionSlot()
{
    clearHashTables();
    addHashTable({HashFunction::Type::Identity, RehashFunction::Type::Identity, 7});
    addHashTable({HashFunction::Type::Identity, RehashFunction::Type::Identity, 11});
    addHashTable({HashFunction::Type::Identity, RehashFunction::Type::Identity, 17});
}


void MainWindow::loadHashConfigurationActionSlot()
{
    clearHashTables();
    addHashTable({HashFunction::Type::Identity, RehashFunction::Type::Identity, 11});
    addHashTable({HashFunction::Type::Square, RehashFunction::Type::Identity, 11});
    addHashTable({HashFunction::Type::Cube, RehashFunction::Type::Identity, 11});
}


void MainWindow::loadRehashConfigurationActionSlot()
{
    clearHashTables();
    addHashTable({HashFunction::Type::Identity, RehashFunction::Type::Identity, 11});
    addHashTable({HashFunction::Type::Identity, RehashFunction::Type::Square, 11});
    addHashTable({HashFunction::Type::Identity, RehashFunction::Type::Cube, 11});
}


void MainWindow::loadSizeOperationListActionSlot()
{
    std::vector<IntAction> actionList
    {
        { ActionType::INSERT, 3 },
        { ActionType::ERASE, 3 },
        { ActionType::INSERT, 13 },
        { ActionType::INSERT, 23 },
        { ActionType::INSERT, 33 },
        { ActionType::INSERT, 43 },
        { ActionType::INSERT, 53 }
    };

    emit loadActionList(actionList);
}


void MainWindow::loadHashOperationListActionSlot()
{
    std::vector<IntAction> actionList
    {
        { ActionType::INSERT, 3 },
        { ActionType::INSERT, 13 },
        { ActionType::INSERT, 23 },
        { ActionType::INSERT, 33 },
        { ActionType::INSERT, 43 },
        { ActionType::INSERT, 53 }
    };
    emit loadActionList(actionList);
}


void MainWindow::loadRehashOperationListActionSlot()
{
    std::vector<IntAction> actionList
    {
        { ActionType::INSERT, 3 },
        { ActionType::INSERT, 13 },
        { ActionType::INSERT, 23 },
        { ActionType::INSERT, 33 },
        { ActionType::INSERT, 43 },
        { ActionType::INSERT, 53 }
    };
    emit loadActionList(actionList);
}


void MainWindow::addHashTableActionSlot()
{
    // TODO
   QDialog *dialog = new QDialog;
   dialog->exec();
   static std::size_t tableSize = 10;
   addHashTable({HashFunction::Type::Identity , RehashFunction::Type::Identity, tableSize++});
}


void MainWindow::removeHashTableActionSlot()
{
    RemoveHashTableDialog *removeHashTableDialog = new RemoveHashTableDialog(_hashTableModelList);
    connect(removeHashTableDialog, &RemoveHashTableDialog::removeHashTable, this, &MainWindow::removeHashTableHandler);
    removeHashTableDialog->exec();
}

