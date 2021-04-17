#pragma once

#include "HashTable.h"
#include "Constants.h"
#include "Action.h"
#include "ActionResult.h"
#include "HashTableDefinition.h"
#include <QAbstractTableModel>


using IntHashTable = HashTable<int>;


class HashTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    HashTableModel(IntHashTable &&hashTable, const HashTableDefinition &definition, QObject *parent = nullptr)
      : QAbstractTableModel(parent)
      , _hashTableDefinition(definition)
      , _hashTable(hashTable)
      , _actionResults()
      , _currentPosition(0)
      , _markPosition(NotInTable)
    {
        _actionResults.push_back(IntActionResult());
        _currentPosition++;
    }

    int rowCount(const QModelIndex &/* parent*/ = QModelIndex()) const { return static_cast<int>(_hashTable.getNumberOfBuckets()); }
    int columnCount(const QModelIndex &/*parent*/ = QModelIndex()) const { return 2; }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QString getHashTableDefinition() const { return _hashTableDefinition.to_string(); }

public slots:
    void loadActionListHandler(const IntActions &actionList);
    void executeActionHandler(IntAction action);
    void undoActionHandler();
    void redoActionHandler();

signals:
    void actionResultCalculated(IntActionResult actionResult);

private:
    using Position = std::size_t;
    HashTableDefinition _hashTableDefinition;
    IntHashTable _hashTable;
    std::vector<IntActionResult> _actionResults;
    Position _currentPosition;
    Position _markPosition;

    const static constexpr Position NotInTable = std::numeric_limits<std::size_t>::max();
};

