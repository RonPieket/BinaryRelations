#pragma once

#include <string>
#include <iostream>
#include "utest.h"
#include "BinaryRelations/BinaryRelations.h"

using namespace BinaryRelations;

UTEST(TestOneToMany, Insert)
{
    OneToMany<int, std::string> otm;
    otm.insert(1, "apple");
    otm.insert(1, "banana");
    otm.insert(2, "cherry");
    otm.insert(3, "date");

    ASSERT_EQ(otm.count(), 4);
    ASSERT_TRUE(otm.contains(1, "apple"));
    ASSERT_TRUE(otm.contains(1, "banana"));
    ASSERT_TRUE(otm.contains(2, "cherry"));
    ASSERT_TRUE(otm.contains(3, "date"));
}

UTEST(TestOneToMany, Erase)
{
    OneToMany<int, std::string> otm;
    otm.insert(1, "apple");
    otm.insert(1, "banana");
    otm.insert(2, "cherry");
    otm.insert(3, "date");
    
    otm.erase(1, "banana");

    ASSERT_EQ(otm.count(), 3);
    ASSERT_TRUE(otm.contains(1, "apple"));
    ASSERT_FALSE(otm.contains(1, "banana"));
    ASSERT_TRUE(otm.contains(2, "cherry"));
    ASSERT_TRUE(otm.contains(3, "date"));
}

UTEST(TestOneToMany, Overwrite)
{
    OneToMany<int, std::string> otm;
    otm.insert(1, "apple");
    otm.insert(1, "banana");
    otm.insert(2, "cherry");
    otm.insert(3, "date");
    
    otm.insert(3, "cherry");
    
    ASSERT_EQ(otm.count(), 4);
    ASSERT_TRUE(otm.contains(1, "apple"));
    ASSERT_TRUE(otm.contains(1, "banana"));
    ASSERT_FALSE(otm.contains(2, "cherry"));
    ASSERT_TRUE(otm.contains(3, "cherry"));
    ASSERT_TRUE(otm.contains(3, "date"));
}

UTEST(TestOneToMany, Iterate)
{
    OneToMany<int, std::string> otm;
    otm.insert(1, "apple");
    otm.insert(1, "banana");
    otm.insert(2, "cherry");
    otm.insert(3, "date");

    int count = 0;
    for (auto p : otm)
        count += 1;
    
    ASSERT_EQ(count, 4);
}

UTEST(TestOneToMany, FindRight)
{
    OneToMany<int, std::string> otm;
    otm.insert(1, "apple");
    otm.insert(1, "banana");
    otm.insert(1, "cherry");
    otm.insert(2, "date");
    otm.insert(3, "elderberry");

    int count = 0;
    for (auto p : *otm.findRight(1))
        count += 1;
    
    ASSERT_EQ(count, 3);
}

UTEST(TestOneToMany, Merge)
{
    OneToMany<int, std::string> otm;
    otm.insert(1, "apple");
    otm.insert(1, "banana");
    otm.insert(2, "cherry");
    otm.insert(3, "date");

    std::vector<OneToMany<int, std::string>::Pair> vec;
    vec.push_back(OneToMany<int, std::string>::Pair(3, "elderberry"));
    vec.push_back(OneToMany<int, std::string>::Pair(4, "fig"));
    
    otm.insert(vec);

    ASSERT_EQ(otm.count(), 6);
    ASSERT_TRUE(otm.contains(1, "apple"));
    ASSERT_TRUE(otm.contains(1, "banana"));
    ASSERT_TRUE(otm.contains(2, "cherry"));
    ASSERT_TRUE(otm.contains(3, "date"));
    ASSERT_TRUE(otm.contains(3, "elderberry"));
    ASSERT_TRUE(otm.contains(4, "fig"));
}

UTEST(TestOneToMany, AllLeft)
{
    OneToMany<int, std::string> otm;
    otm.insert(1, "apple");
    otm.insert(1, "banana");
    otm.insert(1, "cherry");
    otm.insert(2, "date");
    otm.insert(3, "elderberry");

    int count = 0;
    for (auto p : otm.allLeft())
    {
        (void)p; // Shut up compiler
        count += 1;
    }

    ASSERT_EQ(count, 3);
}

UTEST(TestOneToMany, AllRight)
{
    OneToMany<int, std::string> otm;
    otm.insert(1, "apple");
    otm.insert(1, "banana");
    otm.insert(1, "cherry");
    otm.insert(2, "date");
    otm.insert(3, "elderberry");

    int count = 0;
    for (auto p : otm.allRight())
    {
        (void)p; // Shut up compiler
        count += 1;
    }

    ASSERT_EQ(count, 5);
}
