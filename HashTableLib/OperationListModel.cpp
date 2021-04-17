#include "ActionListModel.h"

#include <QString>
#include <QFont>
#include <QBrush>


QVariant ActionListModel::data(const QModelIndex &index, int role) const
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
            return QBrush(Qt::red);
        }
        break;
    case Qt::TextAlignmentRole:
        return static_cast<int>(Qt::AlignHCenter | Qt::AlignVCenter);
        break;
    }
    return QVariant();
}


QVariant ActionListModel::headerData(int section, Qt::Orientation orientation, int role) const
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


void ActionListModel::loadActionListHandler(const IntActions &actionList)
{
    _actionList.clear();
    _actionList.insert(std::end(_actionList), std::begin(actionList), std::end(actionList));
    _currentPosition = 0;
    emit layoutChanged();
    emit currentPositionChanged();
}


void ActionListModel::executeActionHandler(IntAction action)
{
    if (ActionType::INSERT == action.getType() || ActionType::ERASE == action.getType())
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


void ActionListModel::undoActionHandler()
{
    if (_currentPosition > 0)
    {
        _currentPosition--;
        emit layoutChanged();
        emit currentPositionChanged();
        emit undoAction(_currentPosition);
    }
}


void ActionListModel::redoActionHandler()
{
    if (!_actionList.empty() && _currentPosition < _actionList.size())
    {
        _currentPosition++;
        emit layoutChanged();
        emit currentPositionChanged();
        emit redoAction(_currentPosition);
    }
}
