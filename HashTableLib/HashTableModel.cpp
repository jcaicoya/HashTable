#include "HashTableModel.h"

#include <QString>
#include <QFont>
#include <QBrush>
#include <QDebug>
#include <thread>


QVariant HashTableModel::data(const QModelIndex &index, int role) const
{
if (_markPosition != NotInTable)
{
    qDebug() << "Yupieeee!!!";
}
    int row = index.row();
    int col = index.column();
    const int &value = _hashTable.getBucketValue(row);
    using BucketState = array_hash_table::BucketState;
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
        if (static_cast<Position>(row) == _markPosition)
        {
            return QBrush(Qt::yellow);
        }

        switch (state)
        {
        case BucketState::EMPTY:
            return QBrush(Qt::white);
        case BucketState::ACTIVE:
            return QBrush(Qt::green);
        case BucketState::DELETED:
            return QBrush(Qt::blue);
        }
        break;

    case Qt::TextAlignmentRole:
        return static_cast<int>(Qt::AlignHCenter | Qt::AlignVCenter);
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


void HashTableModel::loadOperationListHandler(const IntOperations &actionList)
{
    _hashTable.clear();
    _actionResults.resize(1);
    _currentPosition = 1;

    using Position = array_hash_table::Position;

    for (const auto &action : actionList)
    {
        std::optional<Position> result;
        array_hash_table::IntResultInfo resultInfo;
        bool invalidOperation = false;

        switch (action.getType())
        {
            case OperationType::INSERT:
               result = _hashTable.insertWithInfo(action.getValue(), resultInfo);
            break;
            case OperationType::ERASE:
               result = _hashTable.eraseWithInfo(action.getValue(), resultInfo);
            break;
            default:
                invalidOperation = true;
        }

        if (invalidOperation) continue;

        IntOperationResult actionResult(action, std::move(resultInfo));
        _actionResults.push_back(actionResult);
    }

    _hashTable.clear();
    emit layoutChanged();
    emit actionResultCalculated(_actionResults.front());
}


void HashTableModel::executeOperationHandler(IntOperation action)
{
    static int counter = 1;
    qDebug() << "exectueOperationHandler call: " << counter++;
    using Position = array_hash_table::Position;
    std::optional<Position> result;
    array_hash_table::IntResultInfo resultInfo;

    switch (action.getType())
    {
        case OperationType::INSERT:
           result = _hashTable.insertWithInfo(action.getValue(), resultInfo);
           break;
        case OperationType::ERASE:
           result = _hashTable.eraseWithInfo(action.getValue(), resultInfo);
           break;
        case OperationType::FIND:
           result = _hashTable.findWithInfo(action.getValue(), resultInfo);
           break;
        default:
           return;
    }

    for (auto position : resultInfo._positions)
    {
        int innerCounter = 1;
        _markPosition = position;
 qDebug() << "inner counter: " << innerCounter << " - " << _markPosition;
         /*QModelIndex topLeftIndex = createIndex(_markPosition, 0);
        QModelIndex bottonRightIndex = createIndex(_markPosition + 1, 1);
        emit dataChanged(topLeftIndex, bottonRightIndex, {Qt::BackgroundRole});
        */
        emit layoutChanged();
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    _markPosition = NotInTable;
    emit layoutChanged();


    IntOperationResult actionResult(std::move(action), std::move(resultInfo));

    if (OperationType::INSERT == action.getType() ||
        OperationType::ERASE == action.getType())
    {
        if (_currentPosition < _actionResults.size())
        {
            _actionResults.resize(_currentPosition);
        }
        _actionResults.push_back(actionResult);
        _currentPosition++;
    }

    emit actionResultCalculated(actionResult);
}


void HashTableModel::undoOperationHandler()
{
QString message = "HashTableModel::undoOperationHandler()";
    if (_currentPosition > 0)
    {
        _currentPosition--;
        const auto & actionResult = _actionResults[_currentPosition];
        auto resultType = actionResult.getResultInfo()._resultType;
        if (array_hash_table::ResultType::DONE == resultType)
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

void HashTableModel::redoOperationHandler()
{
QString message = "HashTableModel::redoOperationHandler()";
    if (!_actionResults.empty() && _currentPosition < _actionResults.size())
    {
        _currentPosition++;
        const auto & actionResult = _actionResults[_currentPosition - 1];
        auto resultType = actionResult.getResultInfo()._resultType;
        if (array_hash_table::ResultType::DONE == resultType)
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
