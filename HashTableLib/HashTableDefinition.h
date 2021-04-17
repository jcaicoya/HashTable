#pragma once

#include <HashFunction.h>
#include <RehashFunction.h>
#include <QString>


struct HashTableDefinition
{
    HashTableDefinition(HashFunction::Type hashType, RehashFunction::Type rehashType, std::size_t size)
        : _hashType(hashType)
        , _rehashType(rehashType)
        , _size(size)
    {}

    const HashFunction::Type _hashType;
    const RehashFunction::Type _rehashType;
    const std::size_t _size;

    inline QString to_string() const;
};



inline QString HashTableDefinition::to_string() const
{
    QString hash = HashFunction::to_string(_hashType).data();
    QString rehash = RehashFunction::to_string(_rehashType).data();
    QString size = QString::number(_size);

    return hash + '-' + rehash + '-' + size;
}
