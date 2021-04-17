#pragma once

#include <QWidget>
#include "Operation.h"
#include "OperationList.h"

class QAbstractTableModel;
class QTableView;
class QPushButton;


class OperationListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OperationListWidget(QWidget *parent = nullptr);

    void setModel(QAbstractTableModel *model);

private:
    QTableView *_actionList;
    QPushButton *_undoButton;
    QPushButton *_redoButton;

public slots:
    void currentPositionChangedHandler();

signals:
    void undoOperation();
    void redoOperation();
};
