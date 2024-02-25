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

UTEST(TestOneToMany, BulkInsert)
{
    OneToMany<int, std::string> otm;
    otm.insert(1, "apple");
    otm.insert(1, "apricot");
    otm.insert(1, "avocado");
    
    otm.insert(2, "banana");
    otm.insert(2, "blueberry");
    otm.insert(2, "blackberry");
    
    otm.insert(3, "cherry");
    otm.insert(3, "coconut");
    otm.insert(3, "clementine");
    
    std::vector<OneToMany<int, std::string>::Pair> vec;
    // Add to existing left value
    vec.push_back(OneToMany<int, std::string>::Pair(3, "crabapple"));
    vec.push_back(OneToMany<int, std::string>::Pair(3, "cashew"));
    // Completely new left right pairs
    vec.push_back(OneToMany<int, std::string>::Pair(10, "date"));
    vec.push_back(OneToMany<int, std::string>::Pair(10, "dewberry"));
    // Existing right values, steal them
    vec.push_back(OneToMany<int, std::string>::Pair(10, "apricot"));
    vec.push_back(OneToMany<int, std::string>::Pair(10, "banana"));
    vec.push_back(OneToMany<int, std::string>::Pair(20, "avocado"));
    vec.push_back(OneToMany<int, std::string>::Pair(20, "apple"));
    
    otm.insert(vec);
    
    ASSERT_FALSE(otm.contains(1, "apple"));
    ASSERT_FALSE(otm.contains(1, "apricot"));
    ASSERT_FALSE(otm.contains(1, "avocado"));
    ASSERT_FALSE(otm.contains(2, "banana"));
    ASSERT_TRUE(otm.contains(2, "blueberry"));
    ASSERT_TRUE(otm.contains(2, "blackberry"));
    ASSERT_TRUE(otm.contains(3, "cherry"));
    ASSERT_TRUE(otm.contains(3, "coconut"));
    ASSERT_TRUE(otm.contains(3, "clementine"));

    ASSERT_TRUE(otm.contains(3, "crabapple"));
    ASSERT_TRUE(otm.contains(3, "cashew"));
    ASSERT_TRUE(otm.contains(10, "date"));
    ASSERT_TRUE(otm.contains(10, "dewberry"));
    ASSERT_TRUE(otm.contains(10, "apricot"));
    ASSERT_TRUE(otm.contains(10, "banana"));
    ASSERT_TRUE(otm.contains(20, "avocado"));
    ASSERT_TRUE(otm.contains(20, "apple"));
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
