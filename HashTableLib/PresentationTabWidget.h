#pragma once

#include <QTabWidget>


#include "Operation.h"

class HashTableDefinition;


class PresentationTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit PresentationTabWidget(QWidget *parent = nullptr);

private:
    void setupExample(QString &&title, std::vector<HashTableDefinition> &&definitions, std::vector<IntOperation> &&actionList);
};
