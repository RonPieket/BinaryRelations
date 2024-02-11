#pragma once

#include <string>
#include <iostream>
#include "utest.h"
#include "BinaryRelations/OneToMany.h"

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

UTEST(TestOneToMany, Remove)
{
    OneToMany<int, std::string> otm;
    otm.insert(1, "apple");
    otm.insert(1, "banana");
    otm.insert(2, "cherry");
    otm.insert(3, "date");
    
    otm.remove(1, "banana");

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

    OneToMany<int, std::string> otm2;
    otm2.insert(3, "elderberry");
    otm2.insert(4, "fig");
    
    otm.insert(otm2);

    ASSERT_EQ(otm.count(), 6);
    ASSERT_TRUE(otm.contains(1, "apple"));
    ASSERT_TRUE(otm.contains(1, "banana"));
    ASSERT_TRUE(otm.contains(2, "cherry"));
    ASSERT_TRUE(otm.contains(3, "date"));
    ASSERT_TRUE(otm.contains(3, "elderberry"));
    ASSERT_TRUE(otm.contains(4, "fig"));
}

