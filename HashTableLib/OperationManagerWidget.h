#pragma once

#include "Operation.h"
#include <QWidget>


class ExecuteOperationWidget;
class OperationListWidget;
class OperationListModel;


class OperationManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OperationManagerWidget(QWidget *parent = nullptr);

    void setModel(OperationListModel *actionListModel);

public slots:
    void executeOperationHandler(IntOperation action);
    void undoOperationHandler();
    void redoOperationHandler();

signals:
    void executeOperation(IntOperation action);
    void undoOperation();
    void redoOperation();

private:
    ExecuteOperationWidget *_executeOperationWidget;
    OperationListWidget *_actionListWidget;
};

