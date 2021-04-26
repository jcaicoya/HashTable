#pragma once

#include "Operation.h"
#include <QTabWidget>

class HashTableManagerWidget;
class HashTableModel;
class OperationManagerWidget;
class OperationListModel;
class HashTableDefinition;



class ExampleWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ExampleWidget(std::vector<HashTableDefinition> &&definitions, QWidget *parent = nullptr);
    void loadOperations(std::vector<IntOperation> &&actionList);

signals:
    void loadOperationList(const std::vector<IntOperation> &operationList);

private:
    using HashTableWidgetList = QList<HashTableManagerWidget *>;
    using HashTableModelList = QList<HashTableModel *>;

    OperationManagerWidget *_actionManagerWidget;
    OperationListModel *_actionListModel;
    HashTableWidgetList _hashTableManagerWidgetList;
    HashTableModelList _hashTableModelList;

    void addHashTable(const HashTableDefinition &definition);
};

