#pragma once

#include <string>
#include <iostream>
#include "utest.h"
#include "BinaryRelations/BinaryRelations.h"

using namespace BinaryRelations;

UTEST(TestOneToOne, Insert)
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

UTEST(TestOneToOne, Erase)
{
    OneToOne<int, std::string> oto;
    oto.insert(1, "apple");
    oto.insert(2, "banana");
    oto.insert(3, "cherry");
    oto.insert(4, "date");

    oto.erase(2, "banana");
    oto.erase(3, "foo");

    ASSERT_EQ(oto.count(), 3);
    ASSERT_TRUE(oto.contains(1, "apple"));
    ASSERT_FALSE(oto.contains(2, "banana"));
    ASSERT_TRUE(oto.contains(3, "cherry"));
    ASSERT_TRUE(oto.contains(4, "date"));
}

UTEST(TestOneToOne, Overwrite)
{
    OneToOne<int, std::string> oto;
    oto.insert(1, "apple");
    oto.insert(2, "banana");
    oto.insert(3, "cherry");
    oto.insert(4, "date");
    
    oto.insert(5, "cherry"); // Should erase (3, "cherry")
    
    ASSERT_EQ(oto.count(), 4);
    ASSERT_TRUE(oto.contains(1, "apple"));
    ASSERT_TRUE(oto.contains(2, "banana"));
    ASSERT_FALSE(oto.contains(3, "cherry"));
    ASSERT_TRUE(oto.contains(4, "date"));
    ASSERT_TRUE(oto.contains(5, "cherry"));
}

UTEST(TestOneToOne, Iterate)
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

UTEST(TestOneToOne, Merge)
{
    OneToOne<int, std::string> oto;
    oto.insert(1, "apple");
    oto.insert(2, "banana");
    oto.insert(3, "cherry");
    oto.insert(4, "date");

    OneToOne<int, std::string> oto2;
    oto2.insert(4, "elderberry");
    oto2.insert(5, "fig");
    
    oto.insert(oto2);

    ASSERT_EQ(oto.count(), 5);
    ASSERT_TRUE(oto.contains(1, "apple"));
    ASSERT_TRUE(oto.contains(2, "banana"));
    ASSERT_TRUE(oto.contains(3, "cherry"));
    ASSERT_FALSE(oto.contains(4, "date"));
    ASSERT_TRUE(oto.contains(4, "elderberry"));
    ASSERT_TRUE(oto.contains(5, "fig"));
}

UTEST(TestOneToOne, FindLeft)
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

UTEST(TestOneToOne, FindRight)
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


UTEST(TestOneToOne, AllLeft)
{
    OneToOne<int, std::string> oto;
    oto.insert(1, "apple");
    oto.insert(2, "banana");
    oto.insert(3, "cherry");
    oto.insert(4, "date");
    oto.insert(5, "elderberry");

    int count = 0;
    for (auto p : oto.allLeft())
    {
        (void)p; // Shut up compiler
        count += 1;
    }

    ASSERT_EQ(count, 5);
}

UTEST(TestOneToOne, AllRight)
{
    OneToOne<int, std::string> oto;
    oto.insert(1, "apple");
    oto.insert(2, "banana");
    oto.insert(3, "cherry");
    oto.insert(4, "date");
    oto.insert(5, "elderberry");

    int count = 0;
    for (auto p : oto.allRight())
    {
        (void)p; // Shut up compiler
        count += 1;
    }

    ASSERT_EQ(count, 5);
}

