#pragma once

#include "Operation.h"
#include "OperationList.h"
#include <QObject>
#include <QAbstractTableModel>


class OperationListModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    OperationListModel(QObject *parent = nullptr)
      : QAbstractTableModel(parent)
      , _actionList()
      , _currentPosition(0)
    {}

    int rowCount(const QModelIndex &/* parent*/ = QModelIndex()) const { return static_cast<int>(_actionList.size()); }
    int columnCount(const QModelIndex &/*parent*/ = QModelIndex()) const { return 1; }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    using Position = std::size_t;
    Position getCurrentPosition() const { return _currentPosition; }
    Position getOperationListSize() const { return _actionList.size(); }

public slots:
    void loadOperationListHandler(const IntOperations &actionList);
    void executeOperationHandler(IntOperation action);
    void undoOperationHandler();
    void redoOperationHandler();

signals:
    void currentPositionChanged();
    void undoOperation(OperationListModel::Position position);
    void redoOperation(OperationListModel::Position position);

private:
    std::vector<IntOperation> _actionList;
    Position _currentPosition;
};
