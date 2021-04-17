#include "OperationListWidget.h"
#include <QGroupBox>
#include <QTableView>
#include <QStringList>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QAbstractTableModel>
#include <OperationListModel.h>


OperationListWidget::OperationListWidget(QWidget *parent)
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

    connect(_undoButton, &QPushButton::released, this, &OperationListWidget::undoOperation);
    connect(_redoButton, &QPushButton::released, this, &OperationListWidget::redoOperation);
}


void OperationListWidget::setModel(QAbstractTableModel *model)
{
    _actionList->setModel(model);
}


void OperationListWidget::currentPositionChangedHandler()
{
    IntOperationList::Position currentPosition = static_cast<OperationListModel *>(_actionList->model())->getCurrentPosition();

    const bool undoButtonEnabled = (currentPosition > 0);
    _undoButton->setEnabled(undoButtonEnabled);

    IntOperationList::Position actionListSize = static_cast<OperationListModel *>(_actionList->model())->getOperationListSize();

    const bool redoButtonEnabled = (currentPosition < actionListSize);
    _redoButton->setEnabled(redoButtonEnabled);
}
