#pragma once

#include <QWidget>
#include "Operation.h"

class QAbstractTableModel;
class QTableView;


class HashTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HashTableWidget(QWidget *parent = nullptr);

    void setModel(QAbstractTableModel *model);

private:
    QTableView *_tableView;

};
