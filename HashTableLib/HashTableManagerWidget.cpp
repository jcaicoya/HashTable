#include "HashTableManagerWidget.h"

#include "OperationResultWidget.h"
#include "HashTableWidget.h"
#include "HashTableModel.h"
#include "HashTableDescriptionWidget.h"
#include "HashTableDefinition.h"
#include <QVBoxLayout>


HashTableManagerWidget::HashTableManagerWidget(const HashTableDefinition &description, QWidget *parent)
    : QWidget(parent)
    , _hashTableDescriptionWidget(nullptr)
    , _actionResultWidget(nullptr)
    , _hashTableWidget(nullptr)
{
    _hashTableDescriptionWidget = new HashTableDescriptionWidget(description);
    _actionResultWidget = new OperationResultWidget;
    _hashTableWidget = new HashTableWidget;

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(_hashTableDescriptionWidget);
    layout->addWidget(_actionResultWidget);
    layout->addWidget(_hashTableWidget);
    this->setLayout(layout);

    connect(this, &HashTableManagerWidget::actionResultCalculated, _actionResultWidget, &OperationResultWidget::actionResultCalculatedHandler);
}


void HashTableManagerWidget::setModel(HashTableModel *hashTableModel)
{
   _hashTableWidget->setModel(hashTableModel);
}

void HashTableManagerWidget::actionResultCalculatedHandler(IntOperationResult actionResult)
{
    emit actionResultCalculated(actionResult);
}
