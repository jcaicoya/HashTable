#pragma once

#include "ArrayHashTable.h"
#include "Operation.h"
#include "OperationResult.h"
#include "HashTableDefinition.h"
#include <QAbstractTableModel>
#include <memory>


class HashTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    HashTableModel(std::unique_ptr<IntHashTable> hashTable, const HashTableDefinition &definition, QObject *parent = nullptr)
      : QAbstractTableModel(parent)
      , _hashTableDefinition(definition)
      , _hashTable(std::move(hashTable))
      , _actionResults()
      , _currentPosition(0)
      , _markPosition(NotInTable)
    {
        _actionResults.push_back(IntOperationResult());
        _currentPosition++;
    }

    int rowCount(const QModelIndex &/* parent*/ = QModelIndex()) const { return static_cast<int>(_hashTable->getNumberOfRows()); }
    int columnCount(const QModelIndex &/*parent*/ = QModelIndex()) const { return _hashTable->getNumberOfCols(); }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QString getHashTableDefinition() const { return _hashTableDefinition.to_string(); }

public slots:
    void loadOperationListHandler(const IntOperations &actionList);
    void executeOperationHandler(IntOperation action);
    void undoOperationHandler();
    void redoOperationHandler();

signals:
    void actionResultCalculated(IntOperationResult actionResult);

private:
    using Position = std::size_t;
    HashTableDefinition _hashTableDefinition;
    std::unique_ptr<IntHashTable> _hashTable;
    std::vector<IntOperationResult> _actionResults;
    Position _currentPosition;
    Position _markPosition;

    const static constexpr Position NotInTable = std::numeric_limits<std::size_t>::max();

    QVariant dataForArrayType(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant dataForMatrixType(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerDataForArrayType(int section, Qt::Orientation orientation, int role) const;
    QVariant headerDataForMatrixType(int section, Qt::Orientation orientation, int role) const;
};

