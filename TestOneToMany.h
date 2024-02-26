#pragma once

#include <string>
#include <iostream>
#include <unordered_set>
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

template <typename LeftType, typename RightType>
bool isValid(const OneToMany<LeftType, RightType>& otm)
{
    std::unordered_set<RightType> right_set;
    right_set.insert(otm.allRight().begin(), otm.allRight().end());
    
    for (int left : otm.allLeft())
    {
        for (auto right : *otm.findRight(left))
        {
            auto right_it = right_set.find(right);
            if (right_it == right_set.end())
                return false;
            right_set.erase(right_it);
        }
    }

    return 0 == right_set.size();
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

    ASSERT_EQ(otm.count(), 13);
    ASSERT_TRUE(isValid(otm));

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

UTEST(TestOneToMany, BulkErase)
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
    // Erase all of left=1
    vec.push_back(OneToMany<int, std::string>::Pair(1, "apple"));
    vec.push_back(OneToMany<int, std::string>::Pair(1, "apricot"));
    vec.push_back(OneToMany<int, std::string>::Pair(1, "avocado"));
    // Erase some of left=2
    vec.push_back(OneToMany<int, std::string>::Pair(2, "banana"));
    vec.push_back(OneToMany<int, std::string>::Pair(2, "blueberry"));
    // This pair is not in the set
    vec.push_back(OneToMany<int, std::string>::Pair(1000, "zucchini"));

    otm.erase(vec);

    ASSERT_EQ(otm.count(), 4);
    ASSERT_TRUE(isValid(otm));

    ASSERT_FALSE(otm.contains(1, "apple"));
    ASSERT_FALSE(otm.contains(1, "apricot"));
    ASSERT_FALSE(otm.contains(1, "avocado"));
    ASSERT_FALSE(otm.contains(2, "banana"));
    ASSERT_FALSE(otm.contains(2, "blueberry"));

    ASSERT_TRUE(otm.contains(2, "blackberry"));
    ASSERT_TRUE(otm.contains(3, "cherry"));
    ASSERT_TRUE(otm.contains(3, "coconut"));
    ASSERT_TRUE(otm.contains(3, "clementine"));
}

