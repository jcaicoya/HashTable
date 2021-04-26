#pragma once

#include <QMainWindow>

#define NEW_MAIN_WINDOW

#ifdef NEW_MAIN_WINDOW


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;
};


#else

#include "Operation.h"
#include "OperationResult.h"
#include <QList>

class HashFunction;
class RehashFunction;
class OperationManagerWidget;
class OperationListModel;
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
    void loadOperationList(const std::vector<IntOperation> &actionList);

private:
    void createOperations();
    void addHashTable(const HashTableDefinition &definition);
    void removeHashTable(size_t tableIndex);
    void popHashTable();
    void clearHashTables();

    using HashTableWidgetList = QList<HashTableManagerWidget *>;
    using HashTableModelList = QList<HashTableModel *>;

    OperationManagerWidget *_actionManagerWidget;
    OperationListModel *_actionListModel;
    HashTableWidgetList _hashTableManagerWidgetList;
    HashTableModelList _hashTableModelList;

public slots:
    void removeHashTableHandler(size_t tableIndex);

private slots:
    void example1ActionSlot();
    void loadSizeConfigurationOperationSlot();
    void loadHashConfigurationOperationSlot();
    void loadRehashConfigurationOperationSlot();
    void loadSizeOperationListOperationSlot();
    void loadHashOperationListOperationSlot();
    void loadRehashOperationListOperationSlot();
    void addHashTableOperationSlot();
    void removeHashTableOperationSlot();
    void popHashTableOperationSlot();
    void clearHashTablesOperationSlot();
};

#endif
