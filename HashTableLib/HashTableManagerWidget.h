#pragma once

#include "ActionResult.h"
#include <QWidget>


class ActionResultWidget;
class HashTableWidget;
class HashTableModel;


class HashTableManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HashTableManagerWidget(QWidget *parent = nullptr);

    void setModel(HashTableModel *hashTableModel);

public slots:
    void actionResultCalculatedHandler(IntActionResult actionResult);

signals:
    void actionResultCalculated(IntActionResult actionResult);

private:
    ActionResultWidget *_actionResultWidget;
    HashTableWidget * _hashTableWidget;
};

