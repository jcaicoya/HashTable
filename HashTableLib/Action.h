#pragma once

#include <QString>


enum class ActionType { INSERT, ERASE, FIND, NONE };

inline QString toString(ActionType type)
{
    static QString Insert("INSERT");
    static QString Erase("ERASE");
    static QString Find("FIND");

    switch (type)
    {
        case ActionType::INSERT:
            return Insert;
        case ActionType::ERASE:
            return Erase;
        case ActionType::FIND:
            return Find;
        default:
            return QString();
    }
}


template <typename T>
class Action
{
public:
    Action(ActionType type, const T &value)
        : _type(type)
        , _value(value)
    {}

    ActionType getType() const { return _type; }
    const T & getValue() const { return _value; }
    inline QString toString() const;

    Action()
        : _type(ActionType::NONE)
        , _value()
    {}

private:
    ActionType _type;
    T _value;
};


using IntAction = Action<int>;
using IntActions = std::vector<IntAction>;
using StringAction = Action<QString>;


template <typename T>
inline QString Action<T>::toString() const
{
    return toString(_type) + ' ' + toString(_value);
}


template <>
inline QString Action<QString>::toString() const
{
    return ::toString(_type) + ' ' + _value;
}


template <>
inline QString Action<int>::toString() const
{
    return ::toString(_type) + ' ' + QString::number(_value);
}
