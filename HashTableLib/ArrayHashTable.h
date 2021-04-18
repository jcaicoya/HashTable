#pragma once

#include "HashTable.h"
#include "HashFunction.h"
#include "RehashFunction.h"
#include <string_view>
#include <functional>
#include <vector>
#include <optional>


template <typename T>
class ArrayHashTable : public HashTable<T>
{
public:
    using Position = hash_table::Position;
    using Positions = hash_table::Positions;
    using BucketState = hash_table::BucketState;
    using ResultType = hash_table::ResultType;

    inline ArrayHashTable(const HashFunction &hashFunction, const RehashFunction &rehashFunction, std::size_t size);

    std::optional<Position> insert(const T &value) { return insertImpl(value); }
    std::optional<Position> erase(const T &value) { return eraseImpl(value); }
    std::optional<Position> find(const T &value) const { return findImpl(value); };

    std::optional<Position> insertWithInfo(const T &value, hash_table::ResultInfo<T> &resultInfo) override { return insertImpl(value, &resultInfo); }
    std::optional<Position> eraseWithInfo(const T &value, hash_table::ResultInfo<T> &resultInfo) override { return eraseImpl(value, &resultInfo); }
    std::optional<Position> findWithInfo(const T &value, hash_table::ResultInfo<T> &resultInfo) const override { return findImpl(value, &resultInfo); }

    std::size_t getNumberOfRows() const override { return getNumberOfBuckets(); }
    std::size_t getNumberOfCols() const override { return 2; }
    std::size_t getNumberOfCols(std::size_t /*row*/) const override { return 2; }

    const T & getValue(std::size_t row, std::size_t /*col*/) const override{ return _buckets[row]._value; }
    void setValue(std::size_t row, std::size_t /*col*/, const T &value) override { _buckets[row]._value = value; }
    BucketState getState(std::size_t row,std:: size_t /*col*/) const override { return _buckets[row]._state; }
    void setState(std::size_t row, std::size_t /*col*/, BucketState state) override { _buckets[row]._state = state; }
    void setColumn(std::size_t /*row*/, const hash_table::Column<T> &/*column*/) override { return; }

    inline void clear() override;

    size_t getNumberOfBuckets() const { return _buckets.size(); }
    size_t getNumberOfEmptyBuckets() const { return _numberOfEmptyBuckets; }
    size_t getNumberOfActiveBuckets() const { return _numberOfActiveBuckets; }
    size_t getNumberOfDeletedBuckets() const { return _numberOfDeletedBuckets; }
    double ratio() const { return static_cast<double>(_buckets.size() - _numberOfEmptyBuckets) / _buckets.size(); }

private:
    HashFunction _hashFunction;
    RehashFunction _rehashFunction;
    using Buckets = hash_table::Buckets<T>;
    Buckets _buckets;
    size_t _numberOfEmptyBuckets;
    size_t _numberOfActiveBuckets;
    size_t _numberOfDeletedBuckets;

    inline std::optional<Position> insertImpl(const T &value, hash_table::ResultInfo<T> *resultInfo = nullptr);
    inline std::optional<Position> eraseImpl(const T &value, hash_table::ResultInfo<T> *resultInfo = nullptr);
    inline std::optional<Position> findImpl(const T &valuem, hash_table::ResultInfo<T> *resultInfo = nullptr) const;
};


using IntArrayHashTable = ArrayHashTable<int>;


template <typename T>
inline ArrayHashTable<T>::ArrayHashTable(const HashFunction &hashFunction, const RehashFunction &rehashFunction, std::size_t size)
    : HashTable<T>(hash_table::Type::ARRAY)
    , _hashFunction(hashFunction)
    , _rehashFunction(rehashFunction)
    , _buckets(size)
    , _numberOfEmptyBuckets(size)
    , _numberOfActiveBuckets(0)
    , _numberOfDeletedBuckets(0)
{}


template <typename T>
inline void ArrayHashTable<T>::clear()
{
    for (auto &bucket : _buckets)
    {
        bucket._state = hash_table::BucketState::EMPTY;
    }

    _numberOfEmptyBuckets = _buckets.size();
    _numberOfActiveBuckets = 0;
    _numberOfDeletedBuckets = 0;
}


template <typename T>
inline std::optional<hash_table::Position> ArrayHashTable<T>::insertImpl(const T &value, hash_table::ResultInfo<T> *resultInfo)
{
    const bool calculateResultInfo = (nullptr != resultInfo);

    bool found = false;
    size_t numberOfAttemps = 1;
    const size_t hashPosition = _hashFunction(value) % _buckets.size();
    size_t position = hashPosition;
    if (calculateResultInfo)
    {
        resultInfo->_positions.push_back(position);
    }

    while (!found && numberOfAttemps <= _buckets.size())
    {
        if (BucketState::ACTIVE != _buckets[position]._state)
        {
            found = true;
        }
        else
        {
            position = (hashPosition + _rehashFunction(numberOfAttemps)) % _buckets.size();
            if (calculateResultInfo)
            {
                resultInfo->_positions.push_back(position);
            }
            numberOfAttemps++;
        }
    }

    if (found)
    {
        if (calculateResultInfo)
        {
            resultInfo->_prevBucket = _buckets[position];
        }

        _buckets[position]._value = value;

        if (BucketState::EMPTY == _buckets[position]._state)
        {
            _numberOfEmptyBuckets--;
        }
        else
        {
            _numberOfDeletedBuckets--;
        }

        _buckets[position]._state = BucketState::ACTIVE;
        _numberOfActiveBuckets ++;

        if (calculateResultInfo)
        {
            resultInfo->_resultType = hash_table::ResultType::DONE;
            resultInfo->_currentBucket = _buckets[position];
        }

        return position;
    }
    else
    {
        if (calculateResultInfo)
        {
            resultInfo->_resultType = hash_table::ResultType::OVERFLOWN;
        }

        return std::optional<Position>();
    }
}


template <typename T>
inline std::optional<hash_table::Position> ArrayHashTable<T>::eraseImpl(const T &value, hash_table::ResultInfo<T> *resultInfo)
{
    std::optional<Position> position = findImpl(value, resultInfo);

    if (position.has_value())
    {
        const bool calculateResultInfo = (nullptr != resultInfo);
        if (calculateResultInfo)
        {
            resultInfo->_prevBucket = _buckets[position.value()];
        }

        _buckets[position.value()]._state = BucketState::DELETED;
        _numberOfActiveBuckets--;
        _numberOfDeletedBuckets++;

        if (calculateResultInfo)
        {
            resultInfo->_currentBucket = _buckets[position.value()];
        }
    }

    return position;
}


template <typename T>
inline std::optional<hash_table::Position> ArrayHashTable<T>::findImpl(const T &value, hash_table::ResultInfo<T> *resultInfo) const
{
    const bool calculateResultInfo = (nullptr != resultInfo);

    bool found = false;
    bool couldBeInside = true;
    size_t numberOfAttemps = 1;
    const size_t hashPosition = _hashFunction(value) % _buckets.size();
    size_t position = hashPosition;

    if (calculateResultInfo)
    {
        resultInfo->_positions.push_back(position);
    }

    while (!found && couldBeInside && numberOfAttemps <= _buckets.size())
    {
        if (BucketState::EMPTY == _buckets[position]._state)
        {
            couldBeInside = false;
        }
        else if (BucketState::ACTIVE == _buckets[position]._state && value == _buckets[position]._value)
        {
            found = true;
        }
        else
        {
            position = (hashPosition + _rehashFunction(numberOfAttemps)) % _buckets.size();
            if (calculateResultInfo)
            {
                resultInfo->_positions.push_back(position);
            }
            numberOfAttemps++;
        }
    }

    if (found)
    {
        if (calculateResultInfo)
        {
            resultInfo->_resultType = hash_table::ResultType::DONE;
        }
        return position;
    }
    else
    {
        if (calculateResultInfo)
        {
            if (!couldBeInside)
            {
                resultInfo->_resultType = hash_table::ResultType::NOT_DONE;
            }
            else
            {
                resultInfo->_resultType = hash_table::ResultType::OVERFLOWN;
            }
        }
        return std::optional<Position>();
    }
}
