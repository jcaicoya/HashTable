#pragma once

#include "HashTable.h"
#include "HashTableDefinition.h"


class HashTableFactory
{
public:
    static IntHashTable Build(const HashTableDefinition &definition)
    {
        return IntHashTable(HashFunctionFactory::Build(definition._hashType),
                            RehashFunctionFactory::Build(definition._rehashType),
                            definition._size);
    }
};

