#pragma once

#include <string>
#include <iostream>
#include "utest.h"
#include "BinaryRelations/OneToOne.h"

using namespace BinaryRelations;

UTEST(OneToOneTests, Insert)
{
    OneToOne<int, std::string> oto;
    oto.insert(1, "apple");
    oto.insert(2, "banana");
    oto.insert(3, "cherry");
    oto.insert(4, "date");

    ASSERT_EQ(oto.count(), 4);
    ASSERT_TRUE(oto.contains(1, "apple"));
    ASSERT_TRUE(oto.contains(2, "banana"));
    ASSERT_TRUE(oto.contains(3, "cherry"));
    ASSERT_TRUE(oto.contains(4, "date"));
}

UTEST(OneToOneTests, Remove)
{
    OneToOne<int, std::string> oto;
    oto.insert(1, "apple");
    oto.insert(2, "banana");
    oto.insert(3, "cherry");
    oto.insert(4, "date");

    oto.remove(2, "banana");
    oto.remove(3, "foo");

    ASSERT_EQ(oto.count(), 3);
    ASSERT_TRUE(oto.contains(1, "apple"));
    ASSERT_FALSE(oto.contains(2, "banana"));
    ASSERT_TRUE(oto.contains(3, "cherry"));
    ASSERT_TRUE(oto.contains(4, "date"));
}

UTEST(OneToOneTests, Overwrite)
{
    OneToOne<int, std::string> oto;
    oto.insert(1, "apple");
    oto.insert(2, "banana");
    oto.insert(3, "cherry");
    oto.insert(4, "date");
    
    oto.insert(5, "cherry"); // Should remove (3, "cherry")
    
    ASSERT_EQ(oto.count(), 4);
    ASSERT_TRUE(oto.contains(1, "apple"));
    ASSERT_TRUE(oto.contains(2, "banana"));
    ASSERT_FALSE(oto.contains(3, "cherry"));
    ASSERT_TRUE(oto.contains(4, "date"));
    ASSERT_TRUE(oto.contains(5, "cherry"));
}

UTEST(OneToOneTests, Iterate)
{
    OneToOne<int, std::string> oto;
    oto.insert(1, "apple");
    oto.insert(2, "banana");
    oto.insert(3, "cherry");
    oto.insert(4, "date");
    
    int count = 0;
    for (auto p : oto)
        count += 1;
    
    ASSERT_EQ(count, 4);
}

UTEST(OneToOneTests, Merge)
{
    OneToOne<int, std::string> oto;
    oto.insert(1, "apple");
    oto.insert(2, "banana");
    oto.insert(3, "cherry");
    oto.insert(4, "date");

    OneToOne<int, std::string> oto2;
    oto2.insert(4, "elderberry");
    oto2.insert(5, "fig");
    
    oto.merge(oto2);

    ASSERT_EQ(oto.count(), 5);
    ASSERT_TRUE(oto.contains(1, "apple"));
    ASSERT_TRUE(oto.contains(2, "banana"));
    ASSERT_TRUE(oto.contains(3, "cherry"));
    ASSERT_FALSE(oto.contains(4, "date"));
    ASSERT_TRUE(oto.contains(4, "elderberry"));
    ASSERT_TRUE(oto.contains(5, "fig"));
}

UTEST(OneToOneTests, FindLeft)
{
    OneToOne<int, std::string> oto;
    oto.insert(1, "apple");
    oto.insert(2, "banana");
    oto.insert(3, "cherry");
    oto.insert(4, "date");

    ASSERT_EQ(oto.findLeft("apple", -1), 1);
    ASSERT_EQ(oto.findLeft("banana", -1), 2);
    ASSERT_EQ(oto.findLeft("cherry", -1), 3);
    ASSERT_EQ(oto.findLeft("date", -1), 4);
}

UTEST(OneToOneTests, FindRight)
{
    OneToOne<int, std::string> oto;
    oto.insert(1, "apple");
    oto.insert(2, "banana");
    oto.insert(3, "cherry");
    oto.insert(4, "date");

    ASSERT_TRUE(oto.findRight(1, "not found") == std::string("apple"));
    ASSERT_TRUE(oto.findRight(2, "not found") == std::string("banana"));
    ASSERT_TRUE(oto.findRight(3, "not found") == std::string("cherry"));
    ASSERT_TRUE(oto.findRight(4, "not found") == std::string("date"));
}
