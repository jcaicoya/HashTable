#include "HashTableModel.h"

#include <QString>
#include <QFont>
#include <QBrush>
#include <QDebug>
#include <thread>


QVariant HashTableModel::data(const QModelIndex &index, int role) const
{
    switch (_hashTable->getType())
    {
        case hash_table::Type::ARRAY:
            return dataForArrayType(index, role);
        case hash_table::Type::MATRIX:
            return dataForMatrixType(index, role);
    }
}


QVariant HashTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (_hashTable->getType())
    {
        case hash_table::Type::ARRAY:
            return headerDataForArrayType(section, orientation, role);
        case hash_table::Type::MATRIX:
            return headerDataForMatrixType(section, orientation, role);
    }
}


void HashTableModel::loadOperationListHandler(const IntOperations &actionList)
{
    _hashTable->clear();
    _actionResults.resize(1);
    _currentPosition = 1;

    using Position = hash_table::Position;

    for (const auto &action : actionList)
    {
        std::optional<Position> result;
        hash_table::IntResultInfo resultInfo;
        bool invalidOperation = false;

        switch (action.getType())
        {
            case OperationType::INSERT:
               result = _hashTable->insertWithInfo(action.getValue(), resultInfo);
            break;
            case OperationType::ERASE:
               result = _hashTable->eraseWithInfo(action.getValue(), resultInfo);
            break;
            default:
                invalidOperation = true;
        }

        if (invalidOperation) continue;

        IntOperationResult actionResult(action, std::move(resultInfo));
        _actionResults.push_back(actionResult);
    }

    _hashTable->clear();
    emit layoutChanged();
    emit actionResultCalculated(_actionResults.front());
}


void HashTableModel::executeOperationHandler(IntOperation action)
{
    using Position = hash_table::Position;
    std::optional<Position> result;
    hash_table::IntResultInfo resultInfo;

    switch (action.getType())
    {
        case OperationType::INSERT:
           result = _hashTable->insertWithInfo(action.getValue(), resultInfo);
           break;
        case OperationType::ERASE:
           result = _hashTable->eraseWithInfo(action.getValue(), resultInfo);
           break;
        case OperationType::FIND:
           result = _hashTable->findWithInfo(action.getValue(), resultInfo);
           break;
        default:
           return;
    }

    for (auto position : resultInfo._positions)
    {
        _markPosition = position;
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
            _hashTable->setValue(bucketPostion, 0, prevBucket.value()._value);
            _hashTable->setState(bucketPostion, 1, prevBucket.value()._state);
            emit layoutChanged();
        }

        emit actionResultCalculated(_actionResults[_currentPosition - 1]);
    }
}

void HashTableModel::redoOperationHandler()
{
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
            _hashTable->setValue(bucketPostion, 0, prevBucket.value()._value);
            _hashTable->setState(bucketPostion, 1, prevBucket.value()._state);
            emit layoutChanged();
        }

        emit actionResultCalculated(actionResult);
    }
}


QVariant HashTableModel::dataForArrayType(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    const int &value = _hashTable->getValue(row, 0);
    using BucketState = hash_table::BucketState;
    const BucketState state = _hashTable->getState(row, 1);

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



QVariant HashTableModel::dataForMatrixType(const QModelIndex &index, int role) const
{
    static QFont boldFont;
    boldFont.setBold(true);

    int row = index.row();
    int col = index.column();
    const int &value = _hashTable->getValue(row, col);

    switch (role) {

    case Qt::DisplayRole:
        if (col < static_cast<int>(_hashTable->getNumberOfCols(row)))
        {
            return QString::number(value);
        }
        else
        {
            return QString();
        }

    case Qt::FontRole:
       return boldFont;

    case Qt::BackgroundRole:
        if (col < static_cast<int>(_hashTable->getNumberOfCols(row)))
        {
            return QBrush(Qt::green);
        }
        else
        {
            return QVariant();
        }

    case Qt::TextAlignmentRole:
        return static_cast<int>(Qt::AlignHCenter | Qt::AlignVCenter);
    }

    return QVariant();
}


QVariant HashTableModel::headerDataForArrayType(int section, Qt::Orientation orientation, int role) const
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


QVariant HashTableModel::headerDataForMatrixType(int section, Qt::Orientation /*orientation*/, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    return QString::number(section);
}
