#pragma once

#include "Action.h"
#include "HashTable.h"
#include <optional>


template <typename T>
class ActionResult
{
public:
    using ResultInfo = typename hash_table::ResultInfo<T>;

    ActionResult(Action<T> &&action, ResultInfo &&resultInfo)
        : _action(action)
        , _resultInfo(resultInfo)
    {}

    ActionResult(const Action<T> &action, ResultInfo &&resultInfo)
        : _action(action)
        , _resultInfo(resultInfo)
    {}

    const Action<T> & getAction() const { return _action; }
    const ResultInfo & getResultInfo() const { return _resultInfo; }

    ActionResult()
        : _action()
        , _resultInfo()
    {}

private:
    Action<T> _action;
    ResultInfo _resultInfo;
};


using IntActionResult = ActionResult<int>;

