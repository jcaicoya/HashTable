#include "TestHashTable.h"

#include <HashTableFactory.h>


void TestHashTable::toUpper()
{
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));
}


void TestHashTable::hashTableOperations()
{
    auto hashTable = HashTableFactory::Build({hash_table::Type::ARRAY, HashFunction::Type::Identity, RehashFunction::Type::Next, 5});

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable->insertWithInfo(3, resultInfo);
    QVERIFY(result.has_value());
    QCOMPARE(result.value(), 3ul);
    QCOMPARE(toString(resultInfo._resultType), "DONE");
    QVERIFY(resultInfo._positions.size() == 1ul);
    QCOMPARE(resultInfo._positions.front(), 3ul);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable->findWithInfo(3, resultInfo);
    QVERIFY(result.has_value());
    QCOMPARE(result.value(), 3ul);
    QCOMPARE(toString(resultInfo._resultType), "DONE");
    QVERIFY(resultInfo._positions.size() == 1ul);
    QCOMPARE(resultInfo._positions.front(), 3ul);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable->findWithInfo(4, resultInfo);
    QVERIFY(!result.has_value());
    QCOMPARE(toString(resultInfo._resultType), "NOT_DONE");
    QVERIFY(resultInfo._positions.size() == 1ul);
    QCOMPARE(resultInfo._positions.front(), 4ul);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable->eraseWithInfo(3, resultInfo);
    QVERIFY(result.has_value());
    QCOMPARE(result.value(), 3ul);
    QCOMPARE(toString(resultInfo._resultType), "DONE");
    QVERIFY(resultInfo._positions.size() == 1ul);
    QCOMPARE(resultInfo._positions.front(), 3ul);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable->findWithInfo(3, resultInfo);
    QVERIFY(!result.has_value());
    QCOMPARE(toString(resultInfo._resultType), "NOT_DONE");
    QVERIFY(resultInfo._positions.size() == 2ul);
    QCOMPARE(resultInfo._positions.front(), 3ul);
    QCOMPARE(resultInfo._positions.back(), 4ul);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable->eraseWithInfo(3, resultInfo);
    QVERIFY(!result.has_value());
    QCOMPARE(toString(resultInfo._resultType), "NOT_DONE");
    QVERIFY(resultInfo._positions.size() == 2ul);
    QCOMPARE(resultInfo._positions.front(), 3ul);
    QCOMPARE(resultInfo._positions.back(), 4ul);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable->insertWithInfo(3, resultInfo);
    QVERIFY(result.has_value());
    QCOMPARE(result.value(), 3ul);
    QCOMPARE(toString(resultInfo._resultType), "DONE");
    QVERIFY(resultInfo._positions.size() == 1ul);
    QCOMPARE(resultInfo._positions.front(), 3ul);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable->insertWithInfo(8, resultInfo);
    QVERIFY(result.has_value());
    QCOMPARE(result.value(), 4ul);
    QCOMPARE(toString(resultInfo._resultType), "DONE");
    QVERIFY(resultInfo._positions.size() == 2ul);
    QCOMPARE(resultInfo._positions.front(), 3ul);
    QCOMPARE(resultInfo._positions.back(), 4ul);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable->insertWithInfo(13, resultInfo);
    QVERIFY(result.has_value());
    QCOMPARE(result.value(), 0ul);
    QCOMPARE(toString(resultInfo._resultType), "DONE");
    QVERIFY(resultInfo._positions.size() == 3ul);
    QCOMPARE(resultInfo._positions[0], 3ul);
    QCOMPARE(resultInfo._positions[1], 4ul);
    QCOMPARE(resultInfo._positions[2], 0ul);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable->insertWithInfo(18, resultInfo);
    QVERIFY(result.has_value());
    QCOMPARE(result.value(), 1ul);
    QCOMPARE(toString(resultInfo._resultType), "DONE");
    QVERIFY(resultInfo._positions.size() == 4ul);
    QCOMPARE(resultInfo._positions[0], 3ul);
    QCOMPARE(resultInfo._positions[1], 4ul);
    QCOMPARE(resultInfo._positions[2], 0ul);
    QCOMPARE(resultInfo._positions[3], 1ul);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable->insertWithInfo(23, resultInfo);
    QVERIFY(result.has_value());
    QCOMPARE(result.value(), 2ul);
    QCOMPARE(toString(resultInfo._resultType), "DONE");
    QVERIFY(resultInfo._positions.size() == 5ul);
    QCOMPARE(resultInfo._positions[0], 3ul);
    QCOMPARE(resultInfo._positions[1], 4ul);
    QCOMPARE(resultInfo._positions[2], 0ul);
    QCOMPARE(resultInfo._positions[3], 1ul);
    QCOMPARE(resultInfo._positions[4], 2ul);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable->insertWithInfo(28, resultInfo);
    QVERIFY(!result.has_value());
    QCOMPARE(toString(resultInfo._resultType), "OVERFLOWN");
    QVERIFY(resultInfo._positions.size() == 6ul);
    QCOMPARE(resultInfo._positions[0], 3ul);
    QCOMPARE(resultInfo._positions[1], 4ul);
    QCOMPARE(resultInfo._positions[2], 0ul);
    QCOMPARE(resultInfo._positions[3], 1ul);
    QCOMPARE(resultInfo._positions[4], 2ul);
    QCOMPARE(resultInfo._positions[5], 3ul);
    }
}
