#include "MainWindow.h"
#include "ActionManagerWidget.h"
#include "ActionListModel.h"
#include "HashTableManagerWidget.h"
#include "HashTableModel.h"
#include "HashTableFactory.h"
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>


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


void MainWindow::addHashTable(HashFunction::Type hashFooType, RehashFunction::Type rehashFooType, std::size_t tableSize)
{
    HashTableManagerWidget *hashTableManagerWidget = new HashTableManagerWidget;
    HashTableModel *hashTableModel = new HashTableModel(HashTableFactory::Build(hashFooType, rehashFooType, tableSize));
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


void MainWindow::removeHashTable()
{
    if (_hashTableManagerWidgetList.empty())
    {
        return;
    }

    auto hashTableManagerWidget = _hashTableManagerWidgetList.back();
    _hashTableManagerWidgetList.pop_back();

    auto hashTableModel = _hashTableModelList.back();
    _hashTableModelList.pop_back();

    disconnect(this, &MainWindow::loadActionList, hashTableModel, &HashTableModel::loadActionListHandler);
    disconnect(hashTableModel, &HashTableModel::actionResultCalculated, hashTableManagerWidget, &HashTableManagerWidget::actionResultCalculatedHandler);
    disconnect(_actionManagerWidget, &ActionManagerWidget::executeAction, hashTableModel, &HashTableModel::executeActionHandler);
    disconnect(_actionManagerWidget, &ActionManagerWidget::undoAction, hashTableModel, &HashTableModel::undoActionHandler);
    disconnect(_actionManagerWidget, &ActionManagerWidget::redoAction, hashTableModel, &HashTableModel::redoActionHandler);

    hashTableManagerWidget->deleteLater();
    hashTableModel->deleteLater();
}


void MainWindow::clearHashTables()
{
    while (!_hashTableManagerWidgetList.empty())
    {
        removeHashTable();
    }
}


void MainWindow::loadSizeConfiguration()
{
    clearHashTables();
    addHashTable(HashFunction::Type::Identity, RehashFunction::Type::Identity, 7);
    addHashTable(HashFunction::Type::Identity, RehashFunction::Type::Identity, 11);
    addHashTable(HashFunction::Type::Identity, RehashFunction::Type::Identity, 17);
}


void MainWindow::loadHashConfiguration()
{
    clearHashTables();
    addHashTable(HashFunction::Type::Identity, RehashFunction::Type::Identity, 11);
    addHashTable(HashFunction::Type::Square, RehashFunction::Type::Identity, 11);
    addHashTable(HashFunction::Type::Cube, RehashFunction::Type::Identity, 11);
}


void MainWindow::loadRehashConfiguration()
{
    clearHashTables();
    addHashTable(HashFunction::Type::Identity, RehashFunction::Type::Identity, 11);
    addHashTable(HashFunction::Type::Identity, RehashFunction::Type::Square, 11);
    addHashTable(HashFunction::Type::Identity, RehashFunction::Type::Cube, 11);
}


void MainWindow::loadSizeOperationList()
{
    std::vector<IntAction> actionList
    {
        { ActionType::INSERT, 3 },
        { ActionType::INSERT, 13 },
        { ActionType::INSERT, 23 }
    };

    emit loadActionList(actionList);
}


void MainWindow::loadHashOperationList()
{
    std::vector<IntAction> actionList
    {
        { ActionType::INSERT, 3 },
        { ActionType::INSERT, 13 },
        { ActionType::INSERT, 23 }
    };
    emit loadActionList(actionList);
}


void MainWindow::loadRehashOperationList()
{
    std::vector<IntAction> actionList
    {
        { ActionType::INSERT, 3 },
        { ActionType::INSERT, 13 },
        { ActionType::INSERT, 23 }
    };
    emit loadActionList(actionList);
}


void MainWindow::pushHashTable()
{
   static std::size_t tableSize = 10;
   addHashTable(HashFunction::Type::Identity , RehashFunction::Type::Identity, tableSize++);
}


void MainWindow::popHashTable()
{
   removeHashTable();
}


void MainWindow::createActions()
{
    QMenu *configurationMenu = menuBar()->addMenu(tr("&Configurations"));
    QToolBar *configurationToolBar = addToolBar(tr("Configurations"));

    QAction *sizeConfigurationAction = new QAction(tr("Sizes"), this);
    sizeConfigurationAction->setStatusTip(tr("Load three tables with different sizes"));
    connect(sizeConfigurationAction, &QAction::triggered, this, &MainWindow::loadSizeConfiguration);
    configurationMenu->addAction(sizeConfigurationAction);
    configurationToolBar->addAction(sizeConfigurationAction);

    QAction *hashConfigurationAction = new QAction(tr("Hashes"), this);
    hashConfigurationAction->setStatusTip(tr("Load three tables with different hashes"));
    connect(hashConfigurationAction, &QAction::triggered, this, &MainWindow::loadHashConfiguration);
    configurationMenu->addAction(hashConfigurationAction);
    configurationToolBar->addAction(hashConfigurationAction);

    QAction *rehashConfigurationAction = new QAction(tr("Rehashes"), this);
    hashConfigurationAction->setStatusTip(tr("Load three tables with different rehashes"));
    connect(rehashConfigurationAction, &QAction::triggered, this, &MainWindow::loadRehashConfiguration);
    configurationMenu->addAction(rehashConfigurationAction);
    configurationToolBar->addAction(rehashConfigurationAction);


    QMenu *operationMenu = menuBar()->addMenu(tr("&Operations"));
    QToolBar *operationToolBar = addToolBar(tr("Operations"));

    QAction *sizeOperationAction = new QAction(tr("Sizes"), this);
    sizeOperationAction->setStatusTip(tr("Load operation list example for different sizes"));
    connect(sizeOperationAction, &QAction::triggered, this, &MainWindow::loadSizeOperationList);
    operationMenu->addAction(sizeOperationAction);
    operationToolBar->addAction(sizeOperationAction);

    QAction *hashOperationAction = new QAction(tr("Hashes"), this);
    hashOperationAction->setStatusTip(tr("Load operation example for different hashes"));
    connect(hashOperationAction, &QAction::triggered, this, &MainWindow::loadHashOperationList);
    operationMenu->addAction(hashOperationAction);
    operationToolBar->addAction(hashOperationAction);

    QAction *rehashOperationAction = new QAction(tr("Rehashes"), this);
    hashOperationAction->setStatusTip(tr("Load operation example for different rehashes"));
    connect(rehashOperationAction, &QAction::triggered, this, &MainWindow::loadRehashOperationList);
    operationMenu->addAction(rehashOperationAction);
    operationToolBar->addAction(rehashOperationAction);


    QMenu *hashTableMenu = menuBar()->addMenu(tr("&HashTables"));
    QToolBar *HashTableToolBar = addToolBar(tr("HashTable"));

    QAction *newHashTableAction = new QAction(tr("Push"), this);
    newHashTableAction->setStatusTip(tr("Create a new hash table"));
    connect(newHashTableAction, &QAction::triggered, this, &MainWindow::pushHashTable);
    hashTableMenu->addAction(newHashTableAction);
    HashTableToolBar->addAction(newHashTableAction);

    QAction *deleteHashTableAction = new QAction(tr("Pop"), this);
    deleteHashTableAction->setStatusTip(tr("Remove an existing hash table"));
    connect(deleteHashTableAction, &QAction::triggered, this, &MainWindow::popHashTable);
    hashTableMenu->addAction(deleteHashTableAction);
    HashTableToolBar->addAction(deleteHashTableAction);
}
