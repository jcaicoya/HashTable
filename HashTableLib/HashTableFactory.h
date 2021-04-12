#pragma once

#include "HashTable.h"
#include "HashFunction.h"
#include "RehashFunction.h"


class HashTableFactory
{
public:
    static IntHashTable Build(HashFunction::Type hashType, RehashFunction::Type rehashType, size_t size)
    {
        return IntHashTable(HashFunctionFactory::Build(hashType), RehashFunctionFactory::Build(rehashType), size);
    }
};

