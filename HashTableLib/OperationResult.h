#pragma once

#include "Operation.h"
#include "ArrayHashTable.h"
#include <optional>


template <typename T>
class OperationResult
{
public:
    using ResultInfo = typename array_hash_table::ResultInfo<T>;

    OperationResult(Operation<T> &&action, ResultInfo &&resultInfo)
        : _action(action)
        , _resultInfo(resultInfo)
    {}

    OperationResult(const Operation<T> &action, ResultInfo &&resultInfo)
        : _action(action)
        , _resultInfo(resultInfo)
    {}

    const Operation<T> & getOperation() const { return _action; }
    const ResultInfo & getResultInfo() const { return _resultInfo; }

    OperationResult()
        : _action()
        , _resultInfo()
    {}

private:
    Operation<T> _action;
    ResultInfo _resultInfo;
};


using IntOperationResult = OperationResult<int>;

