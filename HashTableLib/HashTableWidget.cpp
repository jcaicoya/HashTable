#include "HashTableWidget.h"

#include <QTableView>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QAbstractTableModel>


HashTableWidget::HashTableWidget(QWidget *parent)
    : QWidget(parent)
    , _tableView(nullptr)
{
    _tableView = new QTableView;
    _tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(_tableView);

    QGroupBox *widgetBox = new QGroupBox("HASH TABLE");
    widgetBox->setLayout(widgetLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(widgetBox);
    setLayout(mainLayout);
}


void HashTableWidget::setModel(QAbstractTableModel *model)
{
    _tableView->setModel(model);
}
