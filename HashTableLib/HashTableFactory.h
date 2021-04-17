#pragma once

#include "ArrayHashTable.h"
#include "MatrixHashTable.h"
#include "HashTableDefinition.h"
#include <memory>


class HashTableFactory
{
public:
    static std::unique_ptr<IntHashTable> Build(const HashTableDefinition &definition)
    {
        switch (definition._hashTableType)
        {
        case hash_table::Type::ARRAY:
            return std::make_unique<IntArrayHashTable>(HashFunctionFactory::Build(definition._hashFunctionType),
                                                           RehashFunctionFactory::Build(definition._rehashFunctionType),
                                                           definition._size);

        case hash_table::Type::MATRIX:
            return std::make_unique<IntMatrixHashTable>(HashFunctionFactory::Build(definition._hashFunctionType),
                                                        definition._size);
        }
    }
};

