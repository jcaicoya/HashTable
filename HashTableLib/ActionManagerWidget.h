#pragma once

#include "Action.h"
#include <QWidget>


class ExecuteActionWidget;
class ActionListWidget;
class ActionListModel;


class ActionManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ActionManagerWidget(QWidget *parent = nullptr);

    void setModel(ActionListModel *actionListModel);

public slots:
    void executeActionHandler(IntAction action);
    void undoActionHandler();
    void redoActionHandler();

signals:
    void executeAction(IntAction action);
    void undoAction();
    void redoAction();

private:
    ExecuteActionWidget *_executeActionWidget;
    ActionListWidget *_actionListWidget;
};

