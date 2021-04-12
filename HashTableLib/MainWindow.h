#pragma once

#include <QMainWindow>
#include "Action.h"
#include "ActionResult.h"
#include <vector>

class HashFunction;
class RehashFunction;
class ActionManagerWidget;
class ActionListModel;
class HashTableManagerWidget;
class HashTableModel;
class MainModel;



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

signals:
    void loadActionList(const std::vector<IntAction> &actionList);

private slots:
    void loadSizeConfiguration();
    void loadHashConfiguration();
    void loadRehashConfiguration();
    void loadSizeOperationList();
    void loadHashOperationList();
    void loadRehashOperationList();
    void pushHashTable();
    void popHashTable();

private:
    void createActions();
    void addHashTable(HashFunction::Type hashFooType, RehashFunction::Type rehashFooType, std::size_t tableSize);
    void removeHashTable();
    void clearHashTables();

    using HashTableWidgetList = std::vector<HashTableManagerWidget *>;
    using HashTableModelList = std::vector<HashTableModel *>;

    ActionManagerWidget *_actionManagerWidget;
    ActionListModel *_actionListModel;
    HashTableWidgetList _hashTableManagerWidgetList;
    HashTableModelList _hashTableModelList;
};
