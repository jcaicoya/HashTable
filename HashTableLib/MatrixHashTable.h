#pragma once

#include "HashTable.h"
#include "HashFunction.h"
#include <vector>
#include <iterator>

#include <QDebug>


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
    void setColumn(std::size_t row, const hash_table::Column<T> &column) override { _data[row] = column; updateBucketMaxSize(); }

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
    const bool calculateResultInfo = (nullptr != resultInfo);

    if (calculateResultInfo)
    {
        resultInfo->_row = hashPosition;
        resultInfo->_prevColumn = _data[hashPosition];
    }

    _data[hashPosition].push_back(value);


    if (calculateResultInfo)
    {
        const size_t bucketPosition = _data[hashPosition].size() - 1;
        resultInfo->_resultType = hash_table::ResultType::DONE;
        resultInfo->_positions.push_back(hashPosition);
        resultInfo->_positions.push_back(bucketPosition);
        resultInfo->_currentColumn = _data[hashPosition];
    }

    updateBucketMaxSize();
    return hashPosition;
}


template <typename T>
inline std::optional<hash_table::Position> MatrixHashTable<T>::eraseImpl(const T &value, hash_table::ResultInfo<T> *resultInfo)
{
    const bool calculateResultInfo = (nullptr != resultInfo);
    hash_table::ResultInfo<T> eraseResultInfo;

    std::optional<Position> hashPosition = findImpl(value, &eraseResultInfo);

    if (hash_table::ResultType::DONE == eraseResultInfo._resultType)
    {
        eraseResultInfo._prevColumn = _data[hashPosition.value()];
        size_t columnPosition = eraseResultInfo._positions.back();
        _data[hashPosition.value()].erase(_data[hashPosition.value()].begin() + columnPosition);
        eraseResultInfo._currentColumn = _data[hashPosition.value()];
        updateBucketMaxSize();
    }

    if (calculateResultInfo)
    {
        *resultInfo = eraseResultInfo;
    }

    return hashPosition;
}


template <typename T>
inline std::optional<hash_table::Position> MatrixHashTable<T>::findImpl(const T &value, hash_table::ResultInfo<T> *resultInfo) const
{
    const size_t hashPosition = _hashFunction(value) % _data.size();
    resultInfo->_row = hashPosition;
    resultInfo->_positions.push_back(hashPosition);

    const std::vector<T> &column = _data[hashPosition];
    auto itr = std::find(column.begin(), column.end(), value);
    bool found = (column.cend() != itr);

    size_t searchedPosition = 0;
    if (found)
    {
        searchedPosition = std::distance(itr, column.begin());
    }
    else
    {
        if (!column.empty())
        {
            searchedPosition = column.size() - 1;
        }
    }

    if (found || !column.empty())
    {
        for (size_t position = 0; position <= searchedPosition; position++)
        {
            resultInfo->_positions.push_back(position);
        }
    }

    if (found)
    {
        resultInfo->_resultType = hash_table::ResultType::DONE;
        return hashPosition;
    }
    else
    {
        resultInfo->_resultType = hash_table::ResultType::NOT_DONE;
        return std::optional<hash_table::Position>();
    }
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


    qDebug() << "_bucketMaxSize: " << QString::number(_bucketMaxSize);
}
