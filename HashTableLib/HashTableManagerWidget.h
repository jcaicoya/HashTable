#pragma once

#include "HashFunction.h"
#include "RehashFunction.h"
#include "ActionResult.h"
#include <QWidget>

class HashTableDescriptionWidget;
class ActionResultWidget;
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
    void actionResultCalculatedHandler(IntActionResult actionResult);

signals:
    void actionResultCalculated(IntActionResult actionResult);

private:
    HashTableDescriptionWidget *_hashTableDescriptionWidget;
    ActionResultWidget *_actionResultWidget;
    HashTableWidget * _hashTableWidget;
};

