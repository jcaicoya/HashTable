#pragma once

#include <QWidget>
#include "Action.h"
#include "ActionList.h"

class QAbstractTableModel;
class QTableView;
class QPushButton;


class ActionListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ActionListWidget(QWidget *parent = nullptr);

    void setModel(QAbstractTableModel *model);

private:
    QTableView *_actionList;
    QPushButton *_undoButton;
    QPushButton *_redoButton;

public slots:
    void currentPositionChangedHandler();

signals:
    void undoAction();
    void redoAction();
};
