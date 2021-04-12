#include "HashTableManagerWidget.h"

#include "ActionResultWidget.h"
#include "HashTableWidget.h"
#include "HashTableModel.h"
#include <QVBoxLayout>


HashTableManagerWidget::HashTableManagerWidget(QWidget *parent)
    : QWidget(parent)
    , _actionResultWidget(nullptr)
    , _hashTableWidget(nullptr)
{
    _actionResultWidget = new ActionResultWidget;
    _hashTableWidget = new HashTableWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(_actionResultWidget);
    layout->addWidget(_hashTableWidget);
    this->setLayout(layout);

    connect(this, &HashTableManagerWidget::actionResultCalculated, _actionResultWidget, &ActionResultWidget::actionResultCalculatedHandler);
}


void HashTableManagerWidget::setModel(HashTableModel *hashTableModel)
{
   _hashTableWidget->setModel(hashTableModel);
}

void HashTableManagerWidget::actionResultCalculatedHandler(IntActionResult actionResult)
{
    emit actionResultCalculated(actionResult);
}
