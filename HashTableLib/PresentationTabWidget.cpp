#include "PresentationTabWidget.h"

#include "ExampleWidget.h"
#include "HashTableDefinition.h"
#include "Operation.h"
#include "BeginWidget.h"
#include "EndWidget.h"


PresentationTabWidget::PresentationTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    BeginWidget *beginWidget = new BeginWidget("TECH TALKS");
    addTab(beginWidget, "Begin");

    setupExample("Matrix",
                 {
                     {hash_table::Type::MATRIX, HashFunction::Type::Identity, RehashFunction::Type::Next, 10},
                 },
                 {
                     { OperationType::INSERT, 3 },
                     { OperationType::INSERT, 8 },
                     { OperationType::INSERT, 5 },
                     { OperationType::INSERT, 47 },
                     { OperationType::INSERT, 24 },
                     { OperationType::INSERT, 12 },
                     { OperationType::ERASE, 3 },
                     { OperationType::ERASE, 12 }
                 });

    setupExample("Collisions",
                 {
                     {hash_table::Type::MATRIX, HashFunction::Type::Identity, RehashFunction::Type::Next, 10},
                 },
                 {
                     { OperationType::INSERT, 160 },
                     { OperationType::INSERT, 5 },
                     { OperationType::INSERT, 80 },
                     { OperationType::INSERT, 490 },
                     { OperationType::INSERT, 960 },
                     { OperationType::INSERT, 690 },
                     { OperationType::ERASE, 490 },
                     { OperationType::ERASE, 160 }
                 });

    setupExample("Prime size",
                 {
                     {hash_table::Type::MATRIX, HashFunction::Type::Identity, RehashFunction::Type::Next, 11},
                 },
                 {
                     { OperationType::INSERT, 160 },
                     { OperationType::INSERT, 5 },
                     { OperationType::INSERT, 80 },
                     { OperationType::INSERT, 490 },
                     { OperationType::INSERT, 960 },
                     { OperationType::INSERT, 690 },
                     { OperationType::ERASE, 490 },
                     { OperationType::ERASE, 160 }
                 });


    setupExample("Square hash",
                 {
                     {hash_table::Type::MATRIX, HashFunction::Type::Identity, RehashFunction::Type::Next, 31},
                     {hash_table::Type::MATRIX, HashFunction::Type::Square, RehashFunction::Type::Next, 31},
                 },
                 {
                     { OperationType::INSERT, 4 },
                     { OperationType::INSERT, 5 },
                     { OperationType::INSERT, 2 },
                     { OperationType::INSERT, 8 },
                     { OperationType::INSERT, 15 },
                     { OperationType::INSERT, 10 },
                     { OperationType::INSERT, 12 },
                     { OperationType::INSERT, 11 }
                 });


    setupExample("Array",
                 {
                     {hash_table::Type::ARRAY, HashFunction::Type::Identity, RehashFunction::Type::Next, 11},
                 },
                 {
                     { OperationType::INSERT, 38 },
                     { OperationType::INSERT, 22 },
                     { OperationType::INSERT, 69 },
                     { OperationType::INSERT, 87 },
                     { OperationType::INSERT, 52 },
                     { OperationType::INSERT, 6 },
                 });

    setupExample("Collisions II",
                 {
                     {hash_table::Type::ARRAY, HashFunction::Type::Identity, RehashFunction::Type::Next, 11},
                     {hash_table::Type::MATRIX, HashFunction::Type::Identity, RehashFunction::Type::Next, 11},
                 },
                 {
                     { OperationType::INSERT, 23 },
                     { OperationType::INSERT, 56 },
                     { OperationType::INSERT, 89 },
                     { OperationType::INSERT, 12 },
                     { OperationType::INSERT, 45 },
                     { OperationType::INSERT, 1 },
                     { OperationType::ERASE, 12 },
                     { OperationType::ERASE, 89 },
                     { OperationType::INSERT, 78 }
                 });

    setupExample("Square rehash",
                 {
                     {hash_table::Type::ARRAY, HashFunction::Type::Identity, RehashFunction::Type::Square, 11},
                 },
                 {
                     { OperationType::INSERT, 23 },
                     { OperationType::INSERT, 56 },
                     { OperationType::INSERT, 89 },
                     { OperationType::INSERT, 12 },
                     { OperationType::INSERT, 45 },
                     { OperationType::INSERT, 1 },
                     { OperationType::INSERT, 78 }
                 });

    setupExample("Resize",
                 {
                     {hash_table::Type::ARRAY, HashFunction::Type::Identity, RehashFunction::Type::Square, 11},
                     {hash_table::Type::ARRAY, HashFunction::Type::Identity, RehashFunction::Type::Square, 23},
                 },
                 {
                     { OperationType::INSERT, 23 },
                     { OperationType::INSERT, 56 },
                     { OperationType::INSERT, 89 },
                     { OperationType::INSERT, 12 },
                     { OperationType::INSERT, 45 },
                     { OperationType::INSERT, 1 },
                     { OperationType::INSERT, 78 }
                 });

    EndWidget *endWidget = new EndWidget;
    addTab(endWidget, "End");
}


void PresentationTabWidget::setupExample(QString &&title, std::vector<HashTableDefinition> &&definitions, std::vector<IntOperation> &&actionList)
{
    ExampleWidget *exampleWidget = new ExampleWidget(std::move(definitions));
    exampleWidget->loadOperations(std::move(actionList));
    addTab(exampleWidget, title);
}
