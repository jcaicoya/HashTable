#include "HashTableDescriptionWidget.h"

#include "HashTableDefinition.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QGroupBox>


HashTableDescriptionWidget::HashTableDescriptionWidget(const HashTableDefinition &description, QWidget *parent)
    : QWidget(parent)
{
    QString hashInfo = "Hash: ";
    hashInfo += HashFunction::to_string(description._hashType).data();
    QLabel *hashFunctionTypeLabel = new QLabel(hashInfo);

    QString rehashInfo = "Rehash: ";
    rehashInfo += RehashFunction::to_string(description._rehashType).data();
    QLabel *rehashFunctionTypeLabel = new QLabel(rehashInfo);

    QLabel *sizeLabel = new QLabel("Size: " + QString::number(description._size));

    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(hashFunctionTypeLabel);
    widgetLayout->addWidget(rehashFunctionTypeLabel);
    widgetLayout->addWidget(sizeLabel);

    QGroupBox *widgetBox = new QGroupBox("HASH TABLE INFO");
    widgetBox->setLayout(widgetLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(widgetBox);
    setLayout(mainLayout);
}
