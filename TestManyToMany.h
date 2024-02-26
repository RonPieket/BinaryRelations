#pragma once

#include <string>
#include <iostream>
#include "utest.h"
#include "BinaryRelations/BinaryRelations.h"

using namespace BinaryRelations;

UTEST(TestManyToMany, Insert)
{
    ManyToMany<int, std::string> m2m;
    m2m.insert(1, "apple");
    m2m.insert(1, "banana");
    m2m.insert(2, "cherry");
    m2m.insert(3, "cherry");

    ASSERT_EQ(m2m.count(), 4);
    ASSERT_TRUE(m2m.contains(1, "apple"));
    ASSERT_TRUE(m2m.contains(1, "banana"));
    ASSERT_TRUE(m2m.contains(2, "cherry"));
    ASSERT_TRUE(m2m.contains(3, "cherry"));
}

UTEST(TestManyToMany, Erase)
{
    ManyToMany<int, std::string> m2m;
    m2m.insert(1, "apple");
    m2m.insert(1, "banana");
    m2m.insert(2, "cherry");
    m2m.insert(3, "cherry");

    m2m.erase(1, "banana");

    ASSERT_EQ(m2m.count(), 3);
    ASSERT_TRUE(m2m.contains(1, "apple"));
    ASSERT_FALSE(m2m.contains(1, "banana"));
    ASSERT_TRUE(m2m.contains(2, "cherry"));
    ASSERT_TRUE(m2m.contains(3, "cherry"));
}

UTEST(TestManyToMany, Iterate)
{
    ManyToMany<int, std::string> m2m;
    m2m.insert(1, "apple");
    m2m.insert(1, "banana");
    m2m.insert(2, "cherry");
    m2m.insert(3, "cherry");
    
    int count = 0;
    for (auto p : m2m)
        count += 1;
    
    ASSERT_EQ(count, 4);
}

UTEST(TestManyToMany, FindLeft)
{
    ManyToMany<int, std::string> mtm;
    mtm.insert(1, "apple");
    mtm.insert(1, "banana");
    mtm.insert(1, "cherry");
    mtm.insert(2, "date");
    mtm.insert(3, "date");

    int count = 0;
    for (auto p : *mtm.findLeft("date"))
    {
        (void)p; // Shut up compiler
        count += 1;
    }
    
    ASSERT_EQ(count, 2);
}

UTEST(TestManyToMany, FindRight)
{
    ManyToMany<int, std::string> mtm;
    mtm.insert(1, "apple");
    mtm.insert(1, "banana");
    mtm.insert(1, "cherry");
    mtm.insert(2, "date");
    mtm.insert(3, "elderberry");

    int count = 0;
    for (auto p : *mtm.findRight(1))
    {
        (void)p; // Shut up compiler
        count += 1;
    }

    ASSERT_EQ(count, 3);
}

UTEST(TestManyToMany, AllLeft)
{
    ManyToMany<int, std::string> mtm;
    mtm.insert(1, "apple");
    mtm.insert(1, "banana");
    mtm.insert(1, "cherry");
    mtm.insert(2, "date");
    mtm.insert(3, "date");

    int count = 0;
    for (auto p : mtm.allLeft())
    {
        (void)p; // Shut up compiler
        count += 1;
    }

    ASSERT_EQ(count, 3);
}

UTEST(TestManyToMany, AllRight)
{
    ManyToMany<int, std::string> mtm;
    mtm.insert(1, "apple");
    mtm.insert(1, "banana");
    mtm.insert(1, "cherry");
    mtm.insert(2, "date");
    mtm.insert(3, "date");

    int count = 0;
    for (auto p : mtm.allRight())
    {
        (void)p; // Shut up compiler
        count += 1;
    }

    ASSERT_EQ(count, 4);
}

UTEST(TestManyToMany, BulkInsert)
{
    ManyToMany<int, std::string> m2m;
    m2m.insert(1, "apple");
    m2m.insert(1, "apricot");
    m2m.insert(1, "avocado");
    
    m2m.insert(2, "banana");
    m2m.insert(2, "blueberry");
    m2m.insert(2, "blackberry");
    
    m2m.insert(3, "cherry");
    m2m.insert(3, "coconut");
    m2m.insert(3, "clementine");
    
    std::vector<ManyToMany<int, std::string>::Pair> vec;
    vec.push_back(ManyToMany<int, std::string>::Pair(3, "crabapple"));
    vec.push_back(ManyToMany<int, std::string>::Pair(3, "cashew"));
    vec.push_back(ManyToMany<int, std::string>::Pair(10, "date"));
    vec.push_back(ManyToMany<int, std::string>::Pair(10, "dewberry"));
    vec.push_back(ManyToMany<int, std::string>::Pair(10, "apricot"));
    vec.push_back(ManyToMany<int, std::string>::Pair(10, "banana"));
    vec.push_back(ManyToMany<int, std::string>::Pair(20, "avocado"));
    vec.push_back(ManyToMany<int, std::string>::Pair(20, "apple"));
    
    m2m.insert(vec);

    ASSERT_EQ(m2m.count(), 17);
    //ASSERT_TRUE(isValid(m2m));

    ASSERT_TRUE(m2m.contains(1, "apple"));
    ASSERT_TRUE(m2m.contains(1, "apricot"));
    ASSERT_TRUE(m2m.contains(1, "avocado"));
    ASSERT_TRUE(m2m.contains(2, "banana"));
    ASSERT_TRUE(m2m.contains(2, "blueberry"));
    ASSERT_TRUE(m2m.contains(2, "blackberry"));
    ASSERT_TRUE(m2m.contains(3, "cherry"));
    ASSERT_TRUE(m2m.contains(3, "coconut"));
    ASSERT_TRUE(m2m.contains(3, "clementine"));
    ASSERT_TRUE(m2m.contains(3, "crabapple"));
    ASSERT_TRUE(m2m.contains(3, "cashew"));
    ASSERT_TRUE(m2m.contains(10, "date"));
    ASSERT_TRUE(m2m.contains(10, "dewberry"));
    ASSERT_TRUE(m2m.contains(10, "apricot"));
    ASSERT_TRUE(m2m.contains(10, "banana"));
    ASSERT_TRUE(m2m.contains(20, "avocado"));
    ASSERT_TRUE(m2m.contains(20, "apple"));
}

UTEST(TestManyToMany, BulkErase)
{
    ManyToMany<int, std::string> mtm;
    mtm.insert(1, "apple");
    mtm.insert(1, "apricot");
    mtm.insert(1, "avocado");
    
    mtm.insert(2, "banana");
    mtm.insert(2, "blueberry");
    mtm.insert(2, "blackberry");
    
    mtm.insert(3, "cherry");
    mtm.insert(3, "coconut");
    mtm.insert(3, "clementine");
    
    std::vector<ManyToMany<int, std::string>::Pair> vec;
    // Erase all of left=1
    vec.push_back(ManyToMany<int, std::string>::Pair(1, "apple"));
    vec.push_back(ManyToMany<int, std::string>::Pair(1, "apricot"));
    vec.push_back(ManyToMany<int, std::string>::Pair(1, "avocado"));
    // Erase some of left=2
    vec.push_back(ManyToMany<int, std::string>::Pair(2, "banana"));
    vec.push_back(ManyToMany<int, std::string>::Pair(2, "blueberry"));
    // This pair is not in the set
    vec.push_back(ManyToMany<int, std::string>::Pair(1000, "zucchini"));

    mtm.erase(vec);

    ASSERT_EQ(mtm.count(), 4);
    //ASSERT_TRUE(isValid(mtm));

    ASSERT_FALSE(mtm.contains(1, "apple"));
    ASSERT_FALSE(mtm.contains(1, "apricot"));
    ASSERT_FALSE(mtm.contains(1, "avocado"));
    ASSERT_FALSE(mtm.contains(2, "banana"));
    ASSERT_FALSE(mtm.contains(2, "blueberry"));

    ASSERT_TRUE(mtm.contains(2, "blackberry"));
    ASSERT_TRUE(mtm.contains(3, "cherry"));
    ASSERT_TRUE(mtm.contains(3, "coconut"));
    ASSERT_TRUE(mtm.contains(3, "clementine"));
}

