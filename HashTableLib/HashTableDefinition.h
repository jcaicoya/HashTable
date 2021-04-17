#pragma once

#include "HashTable.h"
#include "HashFunction.h"
#include "RehashFunction.h"
#include <QString>


struct HashTableDefinition
{
    HashTableDefinition(hash_table::Type hashTabletype, HashFunction::Type hashFunctionType, RehashFunction::Type rehashFunctionType, std::size_t size)
        : _hashTableType(hashTabletype)
        , _hashFunctionType(hashFunctionType)
        , _rehashFunctionType(rehashFunctionType)
        , _size(size)
    {}

    hash_table::Type _hashTableType;
    HashFunction::Type _hashFunctionType;
    RehashFunction::Type _rehashFunctionType;
    std::size_t _size;

    inline QString to_string() const;
};



inline QString HashTableDefinition::to_string() const
{
    QString type = hash_table::to_string(_hashTableType).data();
    QString hash = HashFunction::to_string(_hashFunctionType).data();
    QString rehash = RehashFunction::to_string(_rehashFunctionType).data();
    QString size = QString::number(_size);

    return type + '-' + hash + '-' + rehash + '-' + size;
}
