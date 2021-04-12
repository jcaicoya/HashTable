#include "TestHashTable.h"

#include <HashTableFactory.h>
#include <Constants.h>


void TestHashTable::toUpper()
{
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));
}


void TestHashTable::hashTableActions()
{
    IntHashTable hashTable = HashTableFactory::Build(HashFunction::Type::Identity, RehashFunction::Type::Identity, 5);

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable.insertWithInfo(3, resultInfo);
    QVERIFY(result.has_value());
    QCOMPARE(result.value(), 3);
    QCOMPARE(toString(resultInfo._resultType), "DONE");
    QVERIFY(resultInfo._positions.size() == 1);
    QCOMPARE(resultInfo._positions.front(), 3);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable.findWithInfo(3, resultInfo);
    QVERIFY(result.has_value());
    QCOMPARE(result.value(), 3);
    QCOMPARE(toString(resultInfo._resultType), "DONE");
    QVERIFY(resultInfo._positions.size() == 1);
    QCOMPARE(resultInfo._positions.front(), 3);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable.findWithInfo(4, resultInfo);
    QVERIFY(!result.has_value());
    QCOMPARE(toString(resultInfo._resultType), "NOT_DONE");
    QVERIFY(resultInfo._positions.size() == 1);
    QCOMPARE(resultInfo._positions.front(), 4);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable.eraseWithInfo(3, resultInfo);
    QVERIFY(result.has_value());
    QCOMPARE(result.value(), 3);
    QCOMPARE(toString(resultInfo._resultType), "DONE");
    QVERIFY(resultInfo._positions.size() == 1);
    QCOMPARE(resultInfo._positions.front(), 3);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable.findWithInfo(3, resultInfo);
    QVERIFY(!result.has_value());
    QCOMPARE(toString(resultInfo._resultType), "NOT_DONE");
    QVERIFY(resultInfo._positions.size() == 2);
    QCOMPARE(resultInfo._positions.front(), 3);
    QCOMPARE(resultInfo._positions.back(), 4);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable.eraseWithInfo(3, resultInfo);
    QVERIFY(!result.has_value());
    QCOMPARE(toString(resultInfo._resultType), "NOT_DONE");
    QVERIFY(resultInfo._positions.size() == 2);
    QCOMPARE(resultInfo._positions.front(), 3);
    QCOMPARE(resultInfo._positions.back(), 4);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable.insertWithInfo(3, resultInfo);
    QVERIFY(result.has_value());
    QCOMPARE(result.value(), 3);
    QCOMPARE(toString(resultInfo._resultType), "DONE");
    QVERIFY(resultInfo._positions.size() == 1);
    QCOMPARE(resultInfo._positions.front(), 3);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable.insertWithInfo(8, resultInfo);
    QVERIFY(result.has_value());
    QCOMPARE(result.value(), 4);
    QCOMPARE(toString(resultInfo._resultType), "DONE");
    QVERIFY(resultInfo._positions.size() == 2);
    QCOMPARE(resultInfo._positions.front(), 3);
    QCOMPARE(resultInfo._positions.back(), 4);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable.insertWithInfo(13, resultInfo);
    QVERIFY(result.has_value());
    QCOMPARE(result.value(), 0);
    QCOMPARE(toString(resultInfo._resultType), "DONE");
    QVERIFY(resultInfo._positions.size() == 3);
    QCOMPARE(resultInfo._positions[0], 3);
    QCOMPARE(resultInfo._positions[1], 4);
    QCOMPARE(resultInfo._positions[2], 0);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable.insertWithInfo(18, resultInfo);
    QVERIFY(result.has_value());
    QCOMPARE(result.value(), 1);
    QCOMPARE(toString(resultInfo._resultType), "DONE");
    QVERIFY(resultInfo._positions.size() == 4);
    QCOMPARE(resultInfo._positions[0], 3);
    QCOMPARE(resultInfo._positions[1], 4);
    QCOMPARE(resultInfo._positions[2], 0);
    QCOMPARE(resultInfo._positions[3], 1);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable.insertWithInfo(23, resultInfo);
    QVERIFY(result.has_value());
    QCOMPARE(result.value(), 2);
    QCOMPARE(toString(resultInfo._resultType), "DONE");
    QVERIFY(resultInfo._positions.size() == 5);
    QCOMPARE(resultInfo._positions[0], 3);
    QCOMPARE(resultInfo._positions[1], 4);
    QCOMPARE(resultInfo._positions[2], 0);
    QCOMPARE(resultInfo._positions[3], 1);
    QCOMPARE(resultInfo._positions[4], 2);
    }

    {
    hash_table::ResultInfo<int> resultInfo;
    auto result = hashTable.insertWithInfo(28, resultInfo);
    QVERIFY(!result.has_value());
    QCOMPARE(toString(resultInfo._resultType), "OVERFLOWN");
    QVERIFY(resultInfo._positions.size() == 6);
    QCOMPARE(resultInfo._positions[0], 3);
    QCOMPARE(resultInfo._positions[1], 4);
    QCOMPARE(resultInfo._positions[2], 0);
    QCOMPARE(resultInfo._positions[3], 1);
    QCOMPARE(resultInfo._positions[4], 2);
    QCOMPARE(resultInfo._positions[5], 3);
    }
}
