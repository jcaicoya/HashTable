#pragma once

#include "HashFunction.h"
#include "RehashFunction.h"
#include <string_view>
#include <functional>
#include <vector>
#include <optional>


namespace array_hash_table
{

enum class BucketState { EMPTY, ACTIVE, DELETED };

inline std::string_view toString(BucketState state)
{
    static std::string_view Empty = "EMPTY";
    static std::string_view Active =  "ACTIVE";
    static std::string_view Deleted = "DELETED";

    switch (state)
    {
    case BucketState::EMPTY:
        return Empty;
    case BucketState::ACTIVE:
        return Active;
    case BucketState::DELETED:
        return Deleted;
    }

    return std::string_view();
}


using Position = std::size_t;
using Positions = std::vector<Position>;


template <typename T>
struct Bucket
{
    Bucket(const T &value = T(), BucketState state = BucketState::EMPTY)
        : _value(value)
        , _state(state)
    {}

    T _value;
    BucketState _state;
};


template <typename T>
using Buckets = std::vector<Bucket<T>>;


enum class ResultType { DONE, NOT_DONE, OVERFLOWN, NONE };


inline std::string_view toString(ResultType resultType)
{
    static std::string_view Done = "DONE";
    static std::string_view NotDone = "NOT_DONE";
    static std::string_view Overflown = "OVERFLOWN";

    switch (resultType)
    {
    case ResultType::DONE:
        return Done;
    case ResultType::NOT_DONE:
        return NotDone;
    case ResultType::OVERFLOWN:
        return Overflown;
    default:
        return std::string_view();
    }
}


template <typename T>
struct ResultInfo
{
    ResultInfo()
        : _resultType(ResultType::NONE)
        , _positions()
        , _prevBucket()
        , _currentBucket()
    {}


    ResultType _resultType;
    Positions _positions;
    std::optional<Bucket<T>> _prevBucket;
    std::optional<Bucket<T>> _currentBucket;
};

using IntResultInfo = ResultInfo<int>;

} // end namespace array_hash_table



template <typename T>
class ArrayHashTable
{
public:
    using Position = array_hash_table::Position;
    using Positions = array_hash_table::Positions;
    using BucketState = array_hash_table::BucketState;
    using ResultType = array_hash_table::ResultType;

    inline ArrayHashTable(HashFunction &&hashFunction, RehashFunction &&rehashFunction, std::size_t size);

    std::optional<Position> insert(const T &value) { return insertImpl(value); }
    std::optional<Position> erase(const T &value) { return eraseImpl(value); }
    std::optional<Position> find(const T &value) const { return findImpl(value); };

    std::optional<Position> insertWithInfo(const T &value, array_hash_table::ResultInfo<T> &resultInfo) { return insertImpl(value, &resultInfo); }
    std::optional<Position> eraseWithInfo(const T &value, array_hash_table::ResultInfo<T> &resultInfo) { return eraseImpl(value, &resultInfo); }
    std::optional<Position> findWithInfo(const T &value, array_hash_table::ResultInfo<T> &resultInfo) const { return findImpl(value, &resultInfo); }

    size_t getNumberOfBuckets() const { return _buckets.size(); }
    size_t getNumberOfEmptyBuckets() const { return _numberOfEmptyBuckets; }
    size_t getNumberOfActiveBuckets() const { return _numberOfActiveBuckets; }
    size_t getNumberOfDeletedBuckets() const { return _numberOfDeletedBuckets; }
    double ration() const { return static_cast<double>(_buckets.size() - _numberOfEmptyBuckets) / _buckets.size(); }

    const T & getBucketValue(Position position) const { return _buckets[position]._value; }
    void setBucketValue(Position position, const T &value) { _buckets[position]._value = value; }

    BucketState getBucketState(Position position) const { return _buckets[position]._state; }
    void setBucketState(Position position, BucketState state) { _buckets[position]._state = state; }

    inline void clear();

private:

    HashFunction _hashFunction;
    RehashFunction _rehashFunction;
    using Buckets = array_hash_table::Buckets<T>;
    Buckets _buckets;
    size_t _numberOfEmptyBuckets;
    size_t _numberOfActiveBuckets;
    size_t _numberOfDeletedBuckets;

    inline std::optional<Position> insertImpl(const T &value, array_hash_table::ResultInfo<T> *resultInfo = nullptr);
    inline std::optional<Position> eraseImpl(const T &value, array_hash_table::ResultInfo<T> *resultInfo = nullptr);
    inline std::optional<Position> findImpl(const T &valuem, array_hash_table::ResultInfo<T> *resultInfo = nullptr) const;
};


using IntHashTable = ArrayHashTable<int>;


template <typename T>
inline ArrayHashTable<T>::ArrayHashTable(HashFunction &&hashFunction, RehashFunction &&rehashFunction, std::size_t size)
    : _hashFunction(hashFunction)
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
        bucket._state = array_hash_table::BucketState::EMPTY;
    }

    _numberOfEmptyBuckets = _buckets.size();
    _numberOfActiveBuckets = 0;
    _numberOfDeletedBuckets = 0;
}


template <typename T>
inline std::optional<array_hash_table::Position> ArrayHashTable<T>::insertImpl(const T &value, array_hash_table::ResultInfo<T> *resultInfo)
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
            resultInfo->_resultType = array_hash_table::ResultType::DONE;
            resultInfo->_currentBucket = _buckets[position];
        }

        return position;
    }
    else
    {
        if (calculateResultInfo)
        {
            resultInfo->_resultType = array_hash_table::ResultType::OVERFLOWN;
        }

        return std::optional<Position>();
    }
}


template <typename T>
inline std::optional<array_hash_table::Position> ArrayHashTable<T>::eraseImpl(const T &value, array_hash_table::ResultInfo<T> *resultInfo)
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
inline std::optional<array_hash_table::Position> ArrayHashTable<T>::findImpl(const T &value, array_hash_table::ResultInfo<T> *resultInfo) const
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
            resultInfo->_resultType = array_hash_table::ResultType::DONE;
        }
        return position;
    }
    else
    {
        if (calculateResultInfo)
        {
            if (!couldBeInside)
            {
                resultInfo->_resultType = array_hash_table::ResultType::NOT_DONE;
            }
            else
            {
                resultInfo->_resultType = array_hash_table::ResultType::OVERFLOWN;
            }
        }
        return std::optional<Position>();
    }
}
