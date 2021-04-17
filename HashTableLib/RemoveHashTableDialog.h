#pragma once

#include <QDialog>
#include <QList>

class HashTableModel;
class QRadioButton;


class RemoveHashTableDialog : public QDialog
{
    Q_OBJECT
public:
    using HashTableModelList = QList<HashTableModel *>;
    explicit RemoveHashTableDialog(const HashTableModelList &tableModelList, QWidget *parent = nullptr);

public slots:
    void verify();

signals:
    void removeHashTable(size_t tableIndex);

private:
    QList<QRadioButton *> _hashTableRadioButtonList;
};

