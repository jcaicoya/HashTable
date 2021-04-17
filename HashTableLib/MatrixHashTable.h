#pragma once

#include "HashTable.h"
#include "HashFunction.h"
#include <vector>
#include <iterator>


template <typename T>
class MatrixHashTable : public HashTable<T>
{
public:
    using Position = hash_table::Position;
    using Positions = hash_table::Positions;
    using ResultType = hash_table::ResultType;

    inline MatrixHashTable(const HashFunction &hashFunction, std::size_t numberOfBuckets);

    std::size_t getBucketMaxSize() const { return _bucketMaxSize; }

    std::optional<Position> insert(const T &value) { return insertImpl(value); }
    std::optional<Position> erase(const T &value) { return eraseImpl(value); }
    std::optional<Position> find(const T &value) const { return findImpl(value); };

    std::optional<Position> insertWithInfo(const T &value, hash_table::ResultInfo<T> &resultInfo) override { return insertImpl(value, &resultInfo); }
    std::optional<Position> eraseWithInfo(const T &value, hash_table::ResultInfo<T> &resultInfo) override { return eraseImpl(value, &resultInfo); }
    std::optional<Position> findWithInfo(const T &value, hash_table::ResultInfo<T> &resultInfo) const override { return findImpl(value, &resultInfo); }

    std::size_t getNumberOfRows() const override { return _data.size(); }
    std::size_t getNumberOfCols() const override { return getBucketMaxSize(); }
    std::size_t getNumberOfCols(const std::size_t row) const override { return _data[row].size(); }

    const T & getValue(std::size_t row, std::size_t col) const override{ return _data[row][col]; }
    void setValue(std::size_t row, std::size_t col, const T &value) override { _data[row][col] = value; }
    hash_table::BucketState getState(std::size_t /*row*/, std::size_t /*col*/) const override { return hash_table::BucketState::EMPTY; }
    void setState(std::size_t /*row*/, std::size_t /*col*/, hash_table::BucketState /*state*/) override { return; }

    inline void clear() override;

private:
    HashFunction _hashFunction;
    using Matrix = std::vector<std::vector<T>>;
    Matrix _data;
    std::size_t _bucketMaxSize;

    inline std::optional<Position> insertImpl(const T &value, hash_table::ResultInfo<T> *resultInfo = nullptr);
    inline std::optional<Position> eraseImpl(const T &value, hash_table::ResultInfo<T> *resultInfo = nullptr);
    inline std::optional<Position> findImpl(const T &valuem, hash_table::ResultInfo<T> *resultInfo = nullptr) const;
    inline void updateBucketMaxSize();
};


using IntMatrixHashTable = MatrixHashTable<int>;



template <typename T>
MatrixHashTable<T>::MatrixHashTable(const HashFunction &hashFunction, std::size_t numberOfBuckets)
    : HashTable<T>(hash_table::Type::MATRIX)
    , _hashFunction(hashFunction)
    , _data(numberOfBuckets)
    , _bucketMaxSize(0)
{}


template <typename T>
inline void MatrixHashTable<T>::clear()
{
    for (auto &buckets : _data)
    {
        buckets.clear();
    }

    _bucketMaxSize = 0;
}


template <typename T>
inline std::optional<hash_table::Position> MatrixHashTable<T>::insertImpl(const T &value, hash_table::ResultInfo<T> *resultInfo)
{
    const size_t hashPosition = _hashFunction(value) % _data.size();
    _data[hashPosition].push_back(value);
    const size_t bucketPosition = _data[hashPosition].size() - 1;

    const bool calculateResultInfo = (nullptr != resultInfo);
    if (calculateResultInfo)
    {
        resultInfo->_resultType = hash_table::ResultType::DONE;
        resultInfo->_positions.push_back(hashPosition);
        resultInfo->_positions.push_back(bucketPosition);
    }

    updateBucketMaxSize();

    return hashPosition;
}


template <typename T>
inline std::optional<hash_table::Position> MatrixHashTable<T>::eraseImpl(const T &value, hash_table::ResultInfo<T> *resultInfo)
{
    std::optional<Position> hashPosition;
    std::optional<Position> bucketPosition;

    if (nullptr != resultInfo)
    {
        hashPosition = findImpl(value, resultInfo);
        if (hashPosition.has_value())
        {
            bucketPosition = resultInfo->_positions.back();
        }
    }
    else
    {
        hash_table::ResultInfo<T> localResultInfo;
        hashPosition = findImpl(value, &localResultInfo);
        if (hashPosition.has_value())
        {
            bucketPosition = resultInfo->_positions.back();
        }
    }

    if (hashPosition.has_value() && bucketPosition.has_value())
    {
        _data[hashPosition.value()].erase(_data[hashPosition.value()].begin() + bucketPosition.value());
        updateBucketMaxSize();
        return hashPosition;
    }

    return std::optional<Position>();
}


template <typename T>
inline std::optional<hash_table::Position> MatrixHashTable<T>::findImpl(const T &value, hash_table::ResultInfo<T> *resultInfo) const
{
    const size_t hashPosition = _hashFunction(value) % _data.size();
    const std::vector<T> &cols = _data[hashPosition];
    auto itr = std::find(cols.begin(), cols.end(), value);
    if (cols.cend() == itr)
    {
        return std::optional<Position>();
    }

    const size_t bucketPosition = std::distance(itr, cols.begin());


    const bool calculateResultInfo = (nullptr != resultInfo);
    if (calculateResultInfo)
    {
        resultInfo->_resultType = hash_table::ResultType::DONE;
        resultInfo->_positions.push_back(hashPosition);
        for (size_t position = 0; position <= bucketPosition; position++)
        {
            resultInfo->_positions.push_back(bucketPosition);
        }
    }

    return hashPosition;
}


template <typename T>
void MatrixHashTable<T>::updateBucketMaxSize()
{
    _bucketMaxSize = 0;

    for (const auto &buckets : _data)
    {
        if (buckets.size() > _bucketMaxSize)
        {
            _bucketMaxSize = buckets.size();
        }
    }
}
