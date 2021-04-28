#include "ExampleTabWidget.h"

#include "ExampleWidget.h"
#include "HashTableDefinition.h"
#include "Operation.h"


ExampleTabWidget::ExampleTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setupExample("Example 1",
                 {
                     {hash_table::Type::MATRIX, HashFunction::Type::Identity, RehashFunction::Type::Next, 4},
                     {hash_table::Type::ARRAY, HashFunction::Type::Identity, RehashFunction::Type::Next, 11}
                 },
                 {
                     { OperationType::INSERT, 3 },
                     { OperationType::INSERT, 13 },
                     { OperationType::INSERT, 23 },
                     { OperationType::INSERT, 33 },
                     { OperationType::INSERT, 43 },
                     { OperationType::INSERT, 53 }
                 });

    setupExample("Example 2",
                 {
                     {hash_table::Type::MATRIX, HashFunction::Type::Identity, RehashFunction::Type::Next, 4},
                     {hash_table::Type::ARRAY, HashFunction::Type::Identity, RehashFunction::Type::Next, 11}
                 },
                 {
                     { OperationType::INSERT, 3 },
                     { OperationType::ERASE, 3 },
                     { OperationType::INSERT, 13 },
                     { OperationType::INSERT, 23 },
                     { OperationType::INSERT, 33 },
                     { OperationType::INSERT, 43 },
                     { OperationType::INSERT, 53 }
                 });
}


void ExampleTabWidget::setupExample(QString &&title, std::vector<HashTableDefinition> &&definitions, std::vector<IntOperation> &&actionList)
{
    ExampleWidget *exampleWidget = new ExampleWidget(std::move(definitions));
    exampleWidget->loadOperations(std::move(actionList));
    addTab(exampleWidget, title);
}
