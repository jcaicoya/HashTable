#include "OperationListModel.h"

#include <QString>
#include <QFont>
#include <QBrush>


QVariant OperationListModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int actionListCurrentPosition = static_cast<int>(_currentPosition);

    switch (role)
    {
    case Qt::DisplayRole:
        return _actionList[row].toString();
        break;
    case Qt::FontRole:
        if (row < actionListCurrentPosition)
        {
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }
        break;
    case Qt::BackgroundRole:
        if (row == actionListCurrentPosition - 1)
        {
            return QBrush(Qt::yellow);
        }
        break;
    case Qt::TextAlignmentRole:
        return static_cast<int>(Qt::AlignHCenter | Qt::AlignVCenter);
        break;
    }
    return QVariant();
}


QVariant OperationListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if (orientation == Qt::Horizontal)
    {
        return "ACTION";
    }
    else
    {
        return QString::number(section + 1);
    }

    return QVariant();
}


void OperationListModel::loadOperationListHandler(const IntOperations &actionList)
{
    _actionList.clear();
    _actionList.insert(std::end(_actionList), std::begin(actionList), std::end(actionList));
    _currentPosition = 0;
    emit layoutChanged();
    emit currentPositionChanged();
}


void OperationListModel::executeOperationHandler(IntOperation action)
{
    if (OperationType::INSERT == action.getType() || OperationType::ERASE == action.getType())
    {
        if (_currentPosition < _actionList.size())
        {
            _actionList.resize(_currentPosition);
        }

        _actionList.push_back(action);
        _currentPosition++;
        emit layoutChanged();
        emit currentPositionChanged();
    }
}


void OperationListModel::undoOperationHandler()
{
    if (_currentPosition > 0)
    {
        _currentPosition--;
        emit layoutChanged();
        emit currentPositionChanged();
        emit undoOperation(_currentPosition);
    }
}


void OperationListModel::redoOperationHandler()
{
    if (!_actionList.empty() && _currentPosition < _actionList.size())
    {
        _currentPosition++;
        emit layoutChanged();
        emit currentPositionChanged();
        emit redoOperation(_currentPosition);
    }
}
