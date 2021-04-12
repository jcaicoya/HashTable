#include "HashTableModel.h"

#include <QString>
#include <QFont>
#include <QBrush>


QVariant HashTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    const int &value = _hashTable.getBucketValue(row);
    using BucketState = hash_table::BucketState;
    const BucketState state = _hashTable.getBucketState(row);

    switch (role) {

    case Qt::DisplayRole:
        if (col == 0)
        {
            if (BucketState::ACTIVE == state)
            {
                return QString::number(value);
            }
            else
            {
                return QString();
            }
        }
        else
        {
            return QString(toString(state).data());
        }

    case Qt::FontRole:
        if (BucketState::ACTIVE == state)
        {
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }
        break;

    case Qt::BackgroundRole:
        switch (state)
        {
        case BucketState::EMPTY:
            return QBrush(Qt::white);
        case BucketState::ACTIVE:
            return QBrush(Qt::green);
        case BucketState::DELETED:
            return QBrush(Qt::yellow);
        }
        break;

    case Qt::TextAlignmentRole:
        return static_cast<int>(Qt::AlignHCenter | Qt::AlignVCenter);
    /*
    case Qt::CheckStateRole:
        if (row == 1 && col == 0) //add a checkbox to cell(1,0)
            return Qt::Checked;
        break;
    */
    }

    return QVariant();
}


QVariant HashTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if (orientation == Qt::Horizontal)
    {
        if (section == 0)
        {
            return "VALUE";
        }
        else
        {
            return "STATE";
        }
    }
    else
    {
        return QString::number(section);
    }

    return QVariant();
}


void HashTableModel::loadActionListHandler(const IntActions &actionList)
{
    _hashTable.clear();
    _actionResults.resize(1);
    _currentPosition = 1;

    using Position = hash_table::Position;

    for (const auto &action : actionList)
    {
        std::optional<Position> result;
        hash_table::IntResultInfo resultInfo;
        bool invalidAction = false;

        switch (action.getType())
        {
            case ActionType::INSERT:
               result = _hashTable.insertWithInfo(action.getValue(), resultInfo);
            break;
            case ActionType::ERASE:
               result = _hashTable.eraseWithInfo(action.getValue(), resultInfo);
            break;
            default:
                invalidAction = true;
        }

        if (invalidAction) continue;

        IntActionResult actionResult(action, std::move(resultInfo));
        _actionResults.push_back(actionResult);
    }

    _hashTable.clear();
    emit layoutChanged();
    emit actionResultCalculated(_actionResults.front());
}


void HashTableModel::executeActionHandler(IntAction action)
{
    using Position = hash_table::Position;
    std::optional<Position> result;
    hash_table::IntResultInfo resultInfo;

    switch (action.getType())
    {
        case ActionType::INSERT:
           result = _hashTable.insertWithInfo(action.getValue(), resultInfo);
           break;
        case ActionType::ERASE:
           result = _hashTable.eraseWithInfo(action.getValue(), resultInfo);
           break;
        case ActionType::FIND:
           result = _hashTable.findWithInfo(action.getValue(), resultInfo);
           break;
        default:
           return;
    }

    IntActionResult actionResult(std::move(action), std::move(resultInfo));

    if (ActionType::INSERT == action.getType() ||
        ActionType::ERASE == action.getType())
    {
        if (_currentPosition < _actionResults.size())
        {
            _actionResults.resize(_currentPosition);
        }
        _actionResults.push_back(actionResult);
        _currentPosition++;
    }


    // TODO: show changes
    /*
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    */
    emit layoutChanged();
    emit actionResultCalculated(actionResult);
}


void HashTableModel::undoActionHandler()
{
QString message = "HashTableModel::undoActionHandler()";
    if (_currentPosition > 0)
    {
        _currentPosition--;
        const auto & actionResult = _actionResults[_currentPosition];
        auto resultType = actionResult.getResultInfo()._resultType;
        if (hash_table::ResultType::DONE == resultType)
        {
            const auto &positions = actionResult.getResultInfo()._positions;
            const auto &prevBucket = actionResult.getResultInfo()._prevBucket;
            auto bucketPostion = positions.back();
            _hashTable.setBucketValue(bucketPostion, prevBucket.value()._value);
            _hashTable.setBucketState(bucketPostion, prevBucket.value()._state);
            emit layoutChanged();
        }

        emit actionResultCalculated(_actionResults[_currentPosition - 1]);
    }
}

void HashTableModel::redoActionHandler()
{
QString message = "HashTableModel::redoActionHandler()";
    if (!_actionResults.empty() && _currentPosition < _actionResults.size())
    {
        _currentPosition++;
        const auto & actionResult = _actionResults[_currentPosition - 1];
        auto resultType = actionResult.getResultInfo()._resultType;
        if (hash_table::ResultType::DONE == resultType)
        {
            const auto &positions = actionResult.getResultInfo()._positions;
            const auto &prevBucket = actionResult.getResultInfo()._currentBucket;
            auto bucketPostion = positions.back();
            _hashTable.setBucketValue(bucketPostion, prevBucket.value()._value);
            _hashTable.setBucketState(bucketPostion, prevBucket.value()._state);
            emit layoutChanged();
        }

        emit actionResultCalculated(actionResult);
    }
}
