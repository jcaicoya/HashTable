#pragma once

#include "Action.h"
#include <vector>



template <typename T>
class ActionList
{
public:
    ActionList()
        : _actions()
        , _currentPosition(0)
    {}

    using Position = std::size_t;

    void add(const Action<T> &action) { _actions.push_back(action); _currentPosition = _actions.size(); }
    std::size_t size() const { return _actions.size(); }
    const Action<T> & get(Position position) const { return _actions[position]; }
    Position getCurrentPosition() const { return _currentPosition; }
    bool undo() { if (_currentPosition > 0) { _currentPosition--; return true; } else return false; }
    bool redo() { if (_currentPosition < _actions.size()) { _currentPosition++; return true;} else return false; }

private:
    using Actions = std::vector<Action<T>>;

    Actions _actions;
    Position _currentPosition;
};

using IntActionList = ActionList<int>;
