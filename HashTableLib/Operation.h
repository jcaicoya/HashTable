#pragma once

#include <QString>
#include <vector>


enum class OperationType { INSERT, ERASE, FIND, NONE };

inline QString toString(OperationType type)
{
    static QString Insert("INSERT");
    static QString Erase("ERASE");
    static QString Find("FIND");

    switch (type)
    {
        case OperationType::INSERT:
            return Insert;
        case OperationType::ERASE:
            return Erase;
        case OperationType::FIND:
            return Find;
        default:
            return QString();
    }
}


template <typename T>
class Operation
{
public:
    Operation(OperationType type, const T &value)
        : _type(type)
        , _value(value)
    {}

    OperationType getType() const { return _type; }
    const T & getValue() const { return _value; }
    inline QString toString() const;

    Operation()
        : _type(OperationType::NONE)
        , _value()
    {}

private:
    OperationType _type;
    T _value;
};


using IntOperation = Operation<int>;
using IntOperations = std::vector<IntOperation>;
using StringOperation = Operation<QString>;


template <typename T>
inline QString Operation<T>::toString() const
{
    return toString(_type) + ' ' + toString(_value);
}


template <>
inline QString Operation<QString>::toString() const
{
    return ::toString(_type) + ' ' + _value;
}


template <>
inline QString Operation<int>::toString() const
{
    return ::toString(_type) + ' ' + QString::number(_value);
}
