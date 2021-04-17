#pragma once

#include "Operation.h"
#include <vector>



template <typename T>
class OperationList
{
public:
    OperationList()
        : _actions()
        , _currentPosition(0)
    {}

    using Position = std::size_t;

    void add(const Operation<T> &action) { _actions.push_back(action); _currentPosition = _actions.size(); }
    std::size_t size() const { return _actions.size(); }
    const Operation<T> & get(Position position) const { return _actions[position]; }
    Position getCurrentPosition() const { return _currentPosition; }
    bool undo() { if (_currentPosition > 0) { _currentPosition--; return true; } else return false; }
    bool redo() { if (_currentPosition < _actions.size()) { _currentPosition++; return true;} else return false; }

private:
    using Operations = std::vector<Operation<T>>;

    Operations _actions;
    Position _currentPosition;
};

using IntOperationList = OperationList<int>;
