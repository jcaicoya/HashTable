#pragma once

#include <QWidget>

class HashTableDefinition;


class HashTableDescriptionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HashTableDescriptionWidget(const HashTableDefinition &definition, QWidget *parent = nullptr);
};

