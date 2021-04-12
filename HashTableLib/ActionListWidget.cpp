#include "ActionListWidget.h"
#include <QGroupBox>
#include <QTableView>
#include <QStringList>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QAbstractTableModel>
#include <ActionListModel.h>


ActionListWidget::ActionListWidget(QWidget *parent)
    : QWidget(parent)
    , _actionList(nullptr)
    , _undoButton(nullptr)
    , _redoButton(nullptr)
{
    _actionList = new QTableView;
    _actionList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    _undoButton = new QPushButton("UNDO");
    _redoButton = new QPushButton("REDO");
    _undoButton->setEnabled(false);
    _redoButton->setEnabled(false);

    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(_actionList);
    widgetLayout->addWidget(_undoButton);
    widgetLayout->addWidget(_redoButton);

    QGroupBox *widgetBox = new QGroupBox("ACTION LIST");
    widgetBox->setLayout(widgetLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(widgetBox);
    setLayout(mainLayout);

    connect(_undoButton, &QPushButton::released, this, &ActionListWidget::undoAction);
    connect(_redoButton, &QPushButton::released, this, &ActionListWidget::redoAction);
}


void ActionListWidget::setModel(QAbstractTableModel *model)
{
    _actionList->setModel(model);
}


void ActionListWidget::currentPositionChangedHandler()
{
    IntActionList::Position currentPosition = static_cast<ActionListModel *>(_actionList->model())->getCurrentPosition();

    const bool undoButtonEnabled = (currentPosition > 0);
    _undoButton->setEnabled(undoButtonEnabled);

    IntActionList::Position actionListSize = static_cast<ActionListModel *>(_actionList->model())->getActionListSize();

    const bool redoButtonEnabled = (currentPosition < actionListSize);
    _redoButton->setEnabled(redoButtonEnabled);
}
