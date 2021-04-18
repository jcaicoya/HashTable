#pragma once

#include <string_view>
#include <vector>
#include <optional>


namespace hash_table
{

enum class Type { ARRAY, MATRIX };

inline std::string_view to_string(Type type)
{
    static std::string_view Array = "ARRAY";
    static std::string_view Matrix =  "MATRIX";

    switch (type)
    {
    case Type::ARRAY:
        return Array;
    case Type::MATRIX:
        return Matrix;
    }

    return std::string_view();
}


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

template <typename T>
using Column = std::vector<T>;


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
        , _row()
        , _prevColumn()
        , _currentColumn()
    {}


    ResultType _resultType;
    Positions _positions;
    std::optional<Bucket<T>> _prevBucket;
    std::optional<Bucket<T>> _currentBucket;
    Position _row;
    std::optional<Column<T>> _prevColumn;
    std::optional<Column<T>> _currentColumn;
};


using IntResultInfo = ResultInfo<int>;

} // end namespace hash_table


template <typename T>
class HashTable
{
public:
    using Type = hash_table::Type;
    Type getType() const { return _type; }

    virtual std::size_t getNumberOfRows() const = 0;
    virtual std::size_t getNumberOfCols() const = 0;
    virtual std::size_t getNumberOfCols(std::size_t row) const = 0;

    virtual const T & getValue(std::size_t row, size_t col) const = 0;
    virtual void setValue(std::size_t row, size_t col, const T &value) = 0;
    virtual hash_table::BucketState getState(std::size_t row, std::size_t col) const = 0;
    virtual void setState(std::size_t row, std::size_t col, hash_table::BucketState bucketState) = 0;
    virtual void setColumn(std::size_t row, const hash_table::Column<T> &column) = 0;

    virtual void clear() = 0;

    virtual std::optional<hash_table::Position> insertWithInfo(const T &value, hash_table::ResultInfo<T> &resultInfo) = 0;
    virtual std::optional<hash_table::Position> eraseWithInfo(const T &value, hash_table::ResultInfo<T> &resultInfo) = 0;
    virtual std::optional<hash_table::Position> findWithInfo(const T &value, hash_table::ResultInfo<T> &resultInfo) const = 0;


    virtual ~HashTable<T>() = default;

protected:
    HashTable(Type type) : _type(type) {}
    const Type _type;
};


using IntHashTable = HashTable<int>;
