#pragma once

#include "HashFunction.h"
#include "RehashFunction.h"
#include "OperationResult.h"
#include <QWidget>

class HashTableDescriptionWidget;
class OperationResultWidget;
class HashTableWidget;
class HashTableModel;
class HashTableDefinition;


class HashTableManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HashTableManagerWidget(const HashTableDefinition &defintion, QWidget *parent = nullptr);

    void setModel(HashTableModel *hashTableModel);

public slots:
    void actionResultCalculatedHandler(IntOperationResult actionResult);

signals:
    void actionResultCalculated(IntOperationResult actionResult);

private:
    HashTableDescriptionWidget *_hashTableDescriptionWidget;
    OperationResultWidget *_actionResultWidget;
    HashTableWidget * _hashTableWidget;
};

