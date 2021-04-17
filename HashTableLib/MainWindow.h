#pragma once

#include <QMainWindow>
#include "Action.h"
#include "ActionResult.h"
#include <QList>

class HashFunction;
class RehashFunction;
class ActionManagerWidget;
class ActionListModel;
class HashTableManagerWidget;
class HashTableModel;
class MainModel;
class HashTableDefinition;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

signals:
    void loadActionList(const std::vector<IntAction> &actionList);

private:
    void createActions();
    void addHashTable(const HashTableDefinition &definition);
    void removeHashTable(size_t tableIndex);
    void popHashTable();
    void clearHashTables();

    using HashTableWidgetList = QList<HashTableManagerWidget *>;
    using HashTableModelList = QList<HashTableModel *>;

    ActionManagerWidget *_actionManagerWidget;
    ActionListModel *_actionListModel;
    HashTableWidgetList _hashTableManagerWidgetList;
    HashTableModelList _hashTableModelList;

public slots:
    void removeHashTableHandler(size_t tableIndex);

private slots:
    void loadSizeConfigurationActionSlot();
    void loadHashConfigurationActionSlot();
    void loadRehashConfigurationActionSlot();
    void loadSizeOperationListActionSlot();
    void loadHashOperationListActionSlot();
    void loadRehashOperationListActionSlot();
    void addHashTableActionSlot();
    void removeHashTableActionSlot();
    void popHashTableActionSlot();
    void clearHashTablesActionSlot();
};
