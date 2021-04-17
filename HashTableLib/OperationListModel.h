#pragma once

#include "Action.h"
#include "ActionList.h"
#include <QObject>
#include <QAbstractTableModel>


class ActionListModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ActionListModel(QObject *parent = nullptr)
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
    Position getActionListSize() const { return _actionList.size(); }

public slots:
    void loadActionListHandler(const IntActions &actionList);
    void executeActionHandler(IntAction action);
    void undoActionHandler();
    void redoActionHandler();

signals:
    void currentPositionChanged();
    void undoAction(ActionListModel::Position position);
    void redoAction(ActionListModel::Position position);

private:
    std::vector<IntAction> _actionList;
    Position _currentPosition;
};
