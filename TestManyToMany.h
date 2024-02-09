#pragma once

#include <string>
#include <iostream>
#include "utest.h"
#include "BinaryRelations/ManyToMany.h"

using namespace BinaryRelations;

UTEST(ManyToManyTests, Insert)
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

UTEST(ManyToManyTests, Remove)
{
    ManyToMany<int, std::string> m2m;
    m2m.insert(1, "apple");
    m2m.insert(1, "banana");
    m2m.insert(2, "cherry");
    m2m.insert(3, "cherry");

    m2m.remove(1, "banana");

    ASSERT_EQ(m2m.count(), 3);
    ASSERT_TRUE(m2m.contains(1, "apple"));
    ASSERT_FALSE(m2m.contains(1, "banana"));
    ASSERT_TRUE(m2m.contains(2, "cherry"));
    ASSERT_TRUE(m2m.contains(3, "cherry"));
}

UTEST(ManyToManyTests, Iterate)
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

UTEST(ManyToManyTests, Merge)
{
    ManyToMany<int, std::string> m2m;
    m2m.insert(1, "apple");
    m2m.insert(1, "banana");
    m2m.insert(2, "cherry");
    m2m.insert(3, "cherry");

    ManyToMany<int, std::string> m2m2;
    m2m2.insert(3, "elderberry");
    m2m2.insert(4, "fig");
    m2m2.insert(1, "apple");

    m2m.merge(m2m2);

    ASSERT_EQ(m2m.count(), 6);
    ASSERT_TRUE(m2m.contains(1, "apple"));
    ASSERT_TRUE(m2m.contains(1, "banana"));
    ASSERT_TRUE(m2m.contains(2, "cherry"));
    ASSERT_TRUE(m2m.contains(3, "cherry"));
    ASSERT_TRUE(m2m.contains(3, "elderberry"));
    ASSERT_TRUE(m2m.contains(4, "fig"));
}

