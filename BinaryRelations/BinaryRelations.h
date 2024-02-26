/*
 MIT License

 Copyright (c) 2024 Ronald Pieket

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

 Latest version at https://github.com/RonPieket/BinaryRelations
 Documentation at https://ronpieket.github.io/BinaryRelations/class_binary_relations_1_1_one_to_many.html

 */

#pragma once

#include <algorithm>
#include <vector>
#include <unordered_map>

namespace BinaryRelations
{
// -------- Manipulate vector with unique sorted elements --------

template <typename T> bool containsInSortedVector(const std::vector<T> *vector, const T &value) noexcept
{
    typename std::vector<T>::const_iterator it = std::lower_bound(vector->cbegin(), vector->cend(), value);
    return it != vector->cend() && *it == value;
}

template <typename T>
typename std::vector<T>::const_iterator findInSortedVector(const std::vector<T> *vector, const T &value) noexcept
{
    typename std::vector<T>::const_iterator it = std::lower_bound(vector->cbegin(), vector->cend(), value);
    if (it != vector->cend() && *it == value)
        return it;
    else
        return vector->cend();
}

template <typename T> int insertIntoSortedVector(std::vector<T> *vector, const T &value) noexcept
{
    typename std::vector<T>::iterator it = std::lower_bound(vector->begin(), vector->end(), value);
    if (it != vector->end() && *it == value)
        return 0; // It's already there
    else
    {
        vector->insert(it, value);
        return 1;
    }
}

template <typename T> int eraseFromSortedVector(std::vector<T> *vector, const T &value) noexcept
{
    typename std::vector<T>::iterator it = std::lower_bound(vector->begin(), vector->end(), value);
    if (it != vector->end() && *it == value)
    {
        vector->erase(it);
        return 1;
    }
    else
        return 0;
}

template <typename T>
void insertIntoSortedVector(const std::vector<T> *sourceVector, const std::vector<T> *insertVector, std::vector<T> *outVector) noexcept
{
    typename std::vector<T>::const_iterator source_it = sourceVector->cbegin();
    typename std::vector<T>::const_iterator insert_it = insertVector->cbegin();
    
    outVector->clear();
    outVector->reserve(sourceVector->size() + insertVector->size());
    
    while (source_it != sourceVector->cend() && insert_it != insertVector->cend())
    {
        if (*source_it == *insert_it)
        {
            outVector->push_back(*source_it++);
            insert_it++;
        }
        else if (*source_it < *insert_it)
        {
            outVector->push_back(*source_it++);
        }
        else
        {
            outVector->push_back(*insert_it++);
        }
    }
    
    while (source_it != sourceVector->cend())
    {
        outVector->push_back(*source_it++);
    }
    
    while (insert_it != insertVector->cend())
    {
        outVector->push_back(*insert_it++);
    }
}

template <typename T>
void eraseFromSortedVector(const std::vector<T> *sourceVector, const std::vector<T> *eraseVector, std::vector<T> *outVector) noexcept
{
    typename std::vector<T>::const_iterator source_it = sourceVector->cbegin();
    typename std::vector<T>::const_iterator erase_it = eraseVector->cbegin();
    typename std::vector<T>::const_iterator source_end = sourceVector->cend();
    typename std::vector<T>::const_iterator erase_end = eraseVector->cend();

    outVector->clear();
    outVector->reserve(std::max(sourceVector->size(), eraseVector->size()));

    while (source_it != source_end && erase_it != erase_end)
    {
        if (*source_it == *erase_it)
        {
            source_it++;
            erase_it++;
        }
        else if (*source_it < *erase_it)
        {
            outVector->push_back(*source_it++);
        }
        else
        {
            outVector->push_back(*erase_it++);
        }
    }

    while (source_it != source_end)
    {
        outVector->push_back(*source_it++);
    }
}

// ----------------------------------------------------------------------------

/// @cond
template <typename KeyType, typename ValueType> class UnorderedMapHelper
{
public:
    const std::unordered_map<KeyType, ValueType>* m_Map;
    
    UnorderedMapHelper(const std::unordered_map<KeyType, ValueType>* map)
    : m_Map(map)
    {}
    
    class Iterator
    {
    public:
        typename std::unordered_map<KeyType, ValueType>::const_iterator it;

        inline KeyType operator*() const noexcept
        {
            return it->first;
        }
        
        inline bool operator==(const Iterator &other) const noexcept
        {
            return it == other.it;
        }
        
        inline bool operator!=(const Iterator &other) const noexcept
        {
            return it != other.it;
        }
        
        inline Iterator operator++() noexcept
        {
            it++;
            return *this;
        }
    };
    
    Iterator begin() const noexcept
    {
        Iterator it;
        it.it = m_Map->begin();
        return it;
    }

    Iterator end() const noexcept
    {
        Iterator it;
        it.it = m_Map->end();
        return it;
    }
};
/// @endcond

// ----------------------------------------------------------------------------

/**
 A one-to-many set of (left, right) pairs. The left side can have any number of right counterparts. The right side can only be in a pair with one left side.
 @image html binary-relations-one-to-many.png "one-to-many"
 */
template <typename LeftType, typename RightType> class OneToMany
{
    std::unordered_map<LeftType, std::vector<RightType> *> m_LeftToRight;
    std::unordered_map<RightType, LeftType> m_RightToLeft;
    std::vector<RightType> m_EmptyRightVector;
    bool m_RightIsSorted = false;

public:
    /**
    @brief A pair of (left, right) values.
     */
    struct Pair
    {
        LeftType left;
        RightType right;
        Pair(LeftType left, RightType right)
        : left(left), right(right)
        {}
        Pair()
        {}
    };

    /**
     @brief Default constructor.
     */
    OneToMany() noexcept
    {}

    /**
     @brief Insert a pair into the set.
     The rule for one-to-many is that if the right value is part of an existing pair in the set, that relation will be erased.
     @param pair The pair to insert.
     */
    void insert(const Pair &pair) noexcept
    {
        insert(pair.left, pair.right);
    }

    /**
     @brief Insert a pair into the set.
     The rule for one-to-many is that if the right value is part of an existing pair in the set, that relation will be erased.
     @param left The left side of the pair to add.
     @param right The right side of the pair to add.
     */
    void insert(const LeftType &left, const RightType &right) noexcept
    {
        auto r2l_it = m_RightToLeft.find(right);
        if (r2l_it != m_RightToLeft.end())
        {
            if (r2l_it->second == left)
                return; // We already have this pair - do nothing

            erase(r2l_it->second, right); // Erase old relation
        }

        auto l2r_vec = m_LeftToRight[left]; // Will insert if it isn't already there.
        if (l2r_vec == nullptr)
        {
            l2r_vec = new std::vector<RightType>();
            m_LeftToRight[left] = l2r_vec;
        }
        insertIntoSortedVector(l2r_vec, right);

        m_RightToLeft[right] = left;
    }

    /**
     @brief Insert multiple  pairs into the set.
     This is faster than inserting the pairs one by one.
     The rule for one-to-many is that if the right value is part of an existing pair in the set, that pair will be erased.
     @param pairs The pairs to add.
     */
    void insert(const std::vector<Pair> &pairs) noexcept
    {
        if(0 == pairs.size())
            return;

        auto compare_left_then_right = [](const Pair &a, const Pair &b)
        {
            if(a.left < b.left) return true;
            if(b.left < a.left) return false;
            return a.right < b.right;
        };

        std::vector<Pair> pairs_to_insert = pairs;  // Deep copy...
        std::sort(pairs_to_insert.begin(), pairs_to_insert.end(), compare_left_then_right); // ...so I can sort

        // ------------------------
        
        std::vector<Pair> pairs_to_erase;
        for (auto pair : pairs_to_insert)
        {
            auto r2l_it = m_RightToLeft.find(pair.right);
            if (r2l_it != m_RightToLeft.end())
            {
                pairs_to_erase.push_back(Pair(r2l_it->second, r2l_it->first));
                m_RightToLeft.erase(r2l_it);
            }
        }

        if (0 != pairs_to_erase.size())
        {
            std::sort(pairs_to_erase.begin(), pairs_to_erase.end(), compare_left_then_right);

            std::vector<RightType> right_to_erase;
            auto end_it = pairs_to_erase.cend();
            for (auto it = pairs_to_erase.cbegin(); it != end_it; )
            {
                // Collect all right values that have the same left value
                right_to_erase.clear();
                auto left = it->left;
                while(it != end_it && it->left == left)
                {
                    right_to_erase.push_back(it->right);
                    it++;
                }

                // Erase them in one go
                auto l2r_it = m_LeftToRight.find(left);
                if (l2r_it != m_LeftToRight.end())
                {
                    auto l2r_vec = l2r_it->second;
                    eraseFromSortedVector(l2r_vec, &right_to_erase, l2r_vec);
                    
                    if(0 == l2r_vec->size())
                    {
                        m_LeftToRight.erase(l2r_it);
                        delete l2r_vec;
                    }
                }
            }
        }

        // -----------------------

        std::vector<RightType> right_to_insert;
        auto it_end = pairs_to_insert.cend();
        for (auto it = pairs_to_insert.cbegin(); it != it_end; )
        {
            // Collect all right values that have the same left value
            right_to_insert.clear();
            auto left = it->left;
            while(it != it_end && it->left == left)
            {
                m_RightToLeft[it->right] = it->left;
                right_to_insert.push_back(it->right);
                it++;
            }

            // Insert them in one go
            auto l2r_it = m_LeftToRight.find(left);
            if (l2r_it != m_LeftToRight.end())
            {
                auto l2r_vec = l2r_it->second;
                auto temp = *l2r_vec;   // Deep copy
                insertIntoSortedVector(&temp, &right_to_insert, l2r_vec);
            }
            else
            {
                // insert new value
                auto l2r_vec = new std::vector<RightType>(right_to_insert);
                m_LeftToRight[left] = l2r_vec;
            }
        }
    }

    /**
     @brief Erase a pair from the set.
     If there is no matching pair in the set, nothing happens.
     @param pair The pair to erase.
     */
    void erase(const Pair &pair) noexcept
    {
        erase(pair.left, pair.right);
    }

    /**
     @brief Erase a pair from the set.
     If there is no matching pair in the set, nothing happens.
     @param left The left side of the pair to erase.
     @param right The right side of the pair to erase.
     */
    void erase(const LeftType &left, const RightType &right) noexcept
    {
        auto r2l_it = m_RightToLeft.find(right);
        if (r2l_it == m_RightToLeft.end())
            return; // (*,right) not in the set - do nothing

        if (r2l_it->second != left)
            return; // (left,right) is not in the set - do nothing

        auto l2r_it = m_LeftToRight.find(left);
        auto l2r_vec = l2r_it->second;
        auto l2r_vec_it = findInSortedVector(l2r_vec, right);
        if (l2r_vec_it != l2r_vec->cend())
        {
            l2r_vec->erase(l2r_vec_it);
            if (l2r_vec->size() == 0)
            {
                m_LeftToRight.erase(l2r_it);
                delete l2r_vec; // Vector is empty now
            }
            m_RightToLeft.erase(r2l_it);
        }
    }

    /**
     @brief Erase all pairs with the given left value.
     If there is no matching pair in the set, nothing happens.
     @param left The left side of the pairs to erase.
     */
    void eraseLeft(const LeftType &left) noexcept
    {
        auto l2r_it = m_LeftToRight.find(left);
        if (l2r_it == m_LeftToRight.end())
            return;

        auto l2r_vec = l2r_it->second;
        for (auto right : l2r_vec)
        {
            auto r2l_it = m_RightToLeft.find(right);
            m_RightToLeft.erase(r2l_it);
        }

        m_LeftToRight.erase(l2r_it);
        delete l2r_vec; // Vector is empty now
    }

    /**
     @brief Erase the pair with the given right value.
     If there is no matching pair in the set, nothing happens.
     @param right The right side of the pairs to erase.
     */
    void eraseRight(const RightType &right) noexcept
    {
        auto r2l_it = m_RightToLeft.find(right);
        if (r2l_it == m_RightToLeft.end())
            return;
        erase(r2l_it->second, right);
    }

    /**
     @brief Erase multiple  pairs from the set.
     This is faster than erasing the pairs one by one.
     @param pairs The pairs to erase.
     */
    void erase(const std::vector<Pair> &pairs) noexcept
    {
        if(0 == pairs.size())
            return;

        auto compare_left_then_right = [](const Pair &a, const Pair &b)
        {
            if(a.left < b.left) return true;
            if(b.left < a.left) return false;
            return a.right < b.right;
        };

        std::vector<Pair> pairs_to_erase = pairs;  // Deep copy...
        std::sort(pairs_to_erase.begin(), pairs_to_erase.end(), compare_left_then_right); // ...so I can sort

        std::vector<RightType> right_to_erase;
        auto end_it = pairs_to_erase.cend();
        for (auto it = pairs_to_erase.cbegin(); it != end_it; )
        {
            // Collect all right values that have the same left value
            right_to_erase.clear();
            auto left = it->left;
            while(it != end_it && it->left == left)
            {
                right_to_erase.push_back(it->right);
                auto r2l_it = m_RightToLeft.find(it->right);
                if (r2l_it != m_RightToLeft.end())
                {
                    m_RightToLeft.erase(r2l_it);
                }
                it++;
            }

            // Erase them in one go
            auto l2r_it = m_LeftToRight.find(left);
            if (l2r_it != m_LeftToRight.end())
            {
                auto l2r_vec = l2r_it->second;
                eraseFromSortedVector(l2r_vec, &right_to_erase, l2r_vec);
                
                if(0 == l2r_vec->size())
                {
                    m_LeftToRight.erase(l2r_it);
                    delete l2r_vec;
                }
            }
        }
    }

    /**
     @brief Erase all pairs from the set.
     */
    void clear() noexcept
    {
        m_RightToLeft.clear();
        m_LeftToRight.clear();
    }

    /**
     @brief Test whether a given pair is in the set.
     @param pair The pair to look for.
     */
    bool contains(const Pair &pair) const noexcept
    {
        return contains(pair.left, pair.right);
    }

    /**
     @brief Test whether a given pair is in the set.
     @param left The left side of the pair to look for.
     @param right The right side of the pair to look for.
     */
    bool contains(const LeftType &left, const RightType &right) const noexcept
    {
        auto r2l_it = m_RightToLeft.find(right);
        return r2l_it != m_RightToLeft.cend() && r2l_it->second == left;
    }

    /**
     @brief Test whether any pair in the set has this left value.
     @param left The left side of the pair to look for.
     */
    bool containsLeft(const LeftType &left) const noexcept
    {
        return m_LeftToRight.contains(left);
    }

    /**
     @brief Test whether any pair in the set has this right value.
     @param right The right side of the pair to look for.
     */
    bool containsRight(const RightType &right) const noexcept
    {
        return m_RightToLeft.contains(right);
    }

    /**
     @brief Find all right values that are paired with this left value.
     If nothing is found, you will get an empty array.
     This works well with range-based-for.
     @param left The left side of the pair to look for.
     @return The vector of right values.
     */
    const std::vector<RightType>* findRight(const LeftType &left) const noexcept
    {
        auto l2r_it = m_LeftToRight.find(left);
        if (l2r_it == m_LeftToRight.end())
            return &m_EmptyRightVector;

        return l2r_it->second;
    }

    /**
     @brief Find the single left value that is paired with this right value.
     If nothing is found, you will get the notFoundValue.
     @param right The right side of the pair to look for.
     @param notFoundValue The value to return if no matching pair is found.
     @return The singular left value.
     */
    LeftType findLeft(const RightType &right, const LeftType &notFoundValue) const noexcept
    {
        auto r2l_it = m_RightToLeft.find(right);
        if (r2l_it == m_RightToLeft.end())
            return notFoundValue;

        return r2l_it->second;
    }

    /**
     @brief Count the number of left values in the set.
     @return The number of left values in the set.
     */
    int countLeft() const noexcept
    {
        return (int)m_LeftToRight.size();
    }

    /**
     @brief Count the number of right values in the set.
     @return The number of right values in the set.
     */
    int countRight() const noexcept
    {
        return (int)m_RightToLeft.size();
    }

    /**
     @brief Count the number of pairs in the set.
     @return The number of pairs in the set.
     */
    int count() const noexcept
    {
        return (int)m_RightToLeft.size();
    }

    /**
     @brief List all left elements.
     @return A helper object to iterate over left elements using range-based-for.
     */
    UnorderedMapHelper<LeftType, std::vector<RightType> *> allLeft() const noexcept
    {
        return UnorderedMapHelper(&m_LeftToRight);
    }

    /**
     @brief List all right elements.
     @return A helper object to iterate over right elements using range-based-for.
     */
    UnorderedMapHelper<RightType, LeftType> allRight() const noexcept
    {
        return UnorderedMapHelper(&m_RightToLeft);
    }

    /**
     @brief A range-based-for compatible iterator.
     */
    class Iterator
    {
        /// @cond
      public:
        typename std::unordered_map<LeftType, std::vector<RightType> *>::const_iterator l2r_it;
        typename std::unordered_map<LeftType, std::vector<RightType> *>::const_iterator l2r_it_end;
        typename std::vector<RightType>::const_iterator l2r_vec_it;

        inline Pair operator*() const noexcept
        {
            auto left = l2r_it->first;
            auto right = *l2r_vec_it;
            return Pair(left, right);
        }

        inline bool operator==(const Iterator &other) const noexcept
        {
            return l2r_it == other.l2r_it;
        }

        inline bool operator!=(const Iterator &other) const noexcept
        {
            return l2r_it != other.l2r_it;
        }

        inline Iterator operator++() noexcept
        {
            l2r_vec_it++;
            if (l2r_vec_it == l2r_it->second->cend())
            {
                l2r_it++;
                if (l2r_it != l2r_it_end)
                {
                    l2r_vec_it = l2r_it->second->cbegin();
                }
            }
            return *this;
        }
        /// @endcond
    };

    /**
     @brief Required member to get range-based-for.
     @return an Iterator set to the first pair in the set.
     */
    Iterator begin() const noexcept
    {
        Iterator it;
        it.l2r_it = m_LeftToRight.cbegin();
        it.l2r_it_end = m_LeftToRight.cend();
        if (it.l2r_it != it.l2r_it_end)
        {
            auto l2r_vec = it.l2r_it->second;
            it.l2r_vec_it = l2r_vec->cbegin();
        }
        return it;
    }

    /**
     @brief Required member to get range-based-for.
     @return an Iterator set to one after the last pair in the set.
     */
    Iterator end() const noexcept
    {
        Iterator it;
        it.l2r_it = m_LeftToRight.cend();
        return it;
    }
};

// ----------------------------------------------------------------------------

/**
 A many-to-many set of (left, right) pairs.
 @image html binary-relations-many-to-many.png "many-to-many"
 */
template <typename LeftType, typename RightType> class ManyToMany
{
    std::unordered_map<LeftType, std::vector<RightType> *> m_LeftToRight;
    std::unordered_map<RightType, std::vector<LeftType> *> m_RightToLeft;
    std::vector<RightType> m_EmptyRightVector;
    std::vector<LeftType> m_EmptyLeftVector;
    int m_Count;

public:
    /**
    @brief A pair of (left, right) values.
     */
    struct Pair
    {
        LeftType left;
        RightType right;
        Pair(LeftType left, RightType right) : left(left), right(right)
        {
        }
        Pair()
        {
        }
    };

    /**
     @brief Default constructor.
     */
    ManyToMany() noexcept
    : m_Count(0)
    {}

    /**
     @brief Insert a pair into the set.
     @param pair The pair to insert.
     */
    void insert(const Pair &pair) noexcept
    {
        insert(pair.left, pair.right);
    }

    /**
     @brief Insert a pair into the set.
     @param left The left side of the pair to add.
     @param right The right side of the pair to add.
     */
    void insert(const LeftType &left, const RightType &right) noexcept
    {
        auto l2r_it = m_LeftToRight.find(left);
        if (l2r_it != m_LeftToRight.end())
        {
            auto l2r_vec = l2r_it->second;
            if (containsInSortedVector(l2r_vec, right))
                return; // We already have this pair;
        }

        auto l2r_vec = m_LeftToRight[left]; // Will insert if it isn't already there.
        if (l2r_vec == nullptr)
        {
            l2r_vec = new std::vector<RightType>();
            m_LeftToRight[left] = l2r_vec;
        }
        insertIntoSortedVector(l2r_vec, right);

        auto r2l_vec = m_RightToLeft[right]; // Will insert if it isn't already there.
        if (r2l_vec == nullptr)
        {
            r2l_vec = new std::vector<LeftType>();
            m_RightToLeft[right] = r2l_vec;
        }
        insertIntoSortedVector(r2l_vec, left);
        m_Count += 1;
    }

    /**
     @brief Insert multiple  pairs into the set.
     @param pairs The pairs to add.
     */
    void insert(const std::vector<Pair> &pairs) noexcept
    {
        if(0 == pairs.size())
            return;

        auto compare_left_then_right = [](const Pair &a, const Pair &b)
        {
            if(a.left < b.left) return true;
            if(b.left < a.left) return false;
            return a.right < b.right;
        };

        auto compare_right_then_left = [](const Pair &a, const Pair &b)
        {
            if(a.right < b.right) return true;
            if(b.right < a.right) return false;
            return a.left < b.left;
        };

        std::vector<Pair> pairs_to_insert = pairs;  // Deep copy...

        // ------------------------

        std::sort(pairs_to_insert.begin(), pairs_to_insert.end(), compare_left_then_right); // ...so I can sort

        std::vector<RightType> right_to_insert;
        auto it_end = pairs_to_insert.cend();
        for (auto it = pairs_to_insert.cbegin(); it != it_end; )
        {
            // Collect all right values that have the same left value
            right_to_insert.clear();
            auto left = it->left;
            while(it != it_end && it->left == left)
            {
                right_to_insert.push_back(it->right);
                it++;
            }

            // Insert them in one go
            auto l2r_it = m_LeftToRight.find(left);
            if (l2r_it != m_LeftToRight.end())
            {
                auto l2r_vec = l2r_it->second;
                auto temp = *l2r_vec;
                m_Count -= l2r_vec->size();
                insertIntoSortedVector(&temp, &right_to_insert, l2r_vec);
                m_Count += l2r_vec->size();
            }
            else
            {
                // insert new value
                auto l2r_vec = new std::vector<RightType>(right_to_insert);
                m_LeftToRight[left] = l2r_vec;
                m_Count += l2r_vec->size();
            }
        }

        // ------------------------

        std::sort(pairs_to_insert.begin(), pairs_to_insert.end(), compare_right_then_left);

        std::vector<LeftType> left_to_insert;
        it_end = pairs_to_insert.cend();
        for (auto it = pairs_to_insert.cbegin(); it != it_end; )
        {
            // Collect all right values that have the same left value
            left_to_insert.clear();
            auto right = it->right;
            while(it != it_end && it->right == right)
            {
                left_to_insert.push_back(it->left);
                it++;
            }

            // Insert them in one go
            auto r2l_it = m_RightToLeft.find(right);
            if (r2l_it != m_RightToLeft.end())
            {
                auto r2l_vec = r2l_it->second;
                auto temp = *r2l_vec;
                insertIntoSortedVector(&temp, &left_to_insert, r2l_vec);
            }
            else
            {
                // insert new value
                auto r2l_vec = new std::vector<LeftType>(left_to_insert);
                m_RightToLeft[right] = r2l_vec;
            }
        }
    }

    /**
     @brief Erase a pair from the set.
     If there is no matching pair in the set, nothing happens.
     @param pair The pair to erase.
     */
    void erase(const Pair &pair) noexcept
    {
        erase(pair.left, pair.right);
    }

    /**
     @brief Erase a pair from the set.
     If there is no matching pair in the set, nothing happens.
     @param left The left side of the pair to erase.
     @param right The right side of the pair to erase.
     */
    void erase(const LeftType &left, const RightType &right) noexcept
    {
        auto l2r_it = m_LeftToRight.find(left);
        if (l2r_it != m_LeftToRight.end())
        {
            auto r2l_it = m_RightToLeft.find(right);
            if (r2l_it != m_RightToLeft.end())
            {
                auto l2r_vec = l2r_it->second;
                auto r2l_vec = r2l_it->second;

                eraseFromSortedVector(l2r_vec, right);
                eraseFromSortedVector(r2l_vec, left);

                if(l2r_vec->size() == 0)
                {
                    m_LeftToRight.erase(l2r_it);
                    delete l2r_vec;
                }

                if(r2l_vec->size() == 0)
                {
                    m_RightToLeft.erase(r2l_it);
                    delete r2l_vec;
                }
                
                m_Count -= 1;
            }
        }
    }

    /**
     @brief Erase all pairs with the given left value.
     If there is no matching pair in the set, nothing happens.
     @param left The left side of the pairs to erase.
     */
    void eraseLeft(const LeftType &left) noexcept
    {
        auto l2r_it = m_LeftToRight.constFind(left);
        if (l2r_it != m_LeftToRight.end())
        {
            auto l2r_vec = l2r_it->second;
            for (auto right : l2r_vec)
            {
                auto r2l_it = m_RightToLeft.constFind(right);
                auto r2l_vec = r2l_it->second;
                eraseFromSortedVector(*r2l_vec, left);
                m_Count -= 1;
                if (r2l_vec->size() == 0)
                {
                    m_RightToLeft.erase(r2l_it);
                    delete r2l_vec;
                }
            }
            m_LeftToRight.erase(l2r_it);
            delete l2r_vec;
        }
    }

    /**
     @brief Erase the pair with the given right value.
     If there is no matching pair in the set, nothing happens.
     @param right The right side of the pairs to erase.
     */
    void eraseRight(const RightType &right) noexcept
    {
        auto r2l_it = m_RightToLeft.constFind(right);
        if (r2l_it != m_RightToLeft.end())
        {
            auto r2l_vec = r2l_it->second;
            for (auto left : r2l_vec)
            {
                auto l2r_it = m_LeftToRight.constFind(left);
                auto l2r_vec = l2r_it->second;
                eraseFromSortedVector(*l2r_vec, right);
                m_Count -= 1;
                if (l2r_vec->size() == 0)
                {
                    m_LeftToRight.erase(l2r_it);
                    delete l2r_vec;
                }
            }
            m_RightToLeft.erase(r2l_it);
            delete r2l_vec;
        }
    }

    /**
     @brief Erase multiple pairs from the set.
     @param other The set of pairs to erase.
     */
    void erase(const ManyToMany<LeftType, RightType>& other) noexcept
    {
        for (auto pair : other)
        {
            erase(pair);
        }
    }

    /**
     @brief Erase all pairs from the set.
     */
    void clear() noexcept
    {
        m_RightToLeft.clear();
        m_LeftToRight.clear();
    }

    /**
     @brief Test whether a given pair is in the set.
     @param pair The pair to look for.
     */
    bool contains(const Pair &pair) const noexcept
    {
        return contains(pair.left, pair.right);
    }

    /**
     @brief Test whether a given pair is in the set.
     @param left The left side of the pair to look for.
     @param right The right side of the pair to look for.
     */
    bool contains(const LeftType &left, const RightType &right) const noexcept
    {
        auto l2r_it = m_LeftToRight.find(left);
        if (l2r_it != m_LeftToRight.end())
        {
            auto r2l_it = m_RightToLeft.find(right);
            if (r2l_it != m_RightToLeft.end())
            {
                auto l2r_vec = l2r_it->second;
                auto r2l_vec = r2l_it->second;
                if (l2r_vec->size() < r2l_vec->size())
                {
                    return containsInSortedVector(l2r_vec, right);
                }
                else
                {
                    return containsInSortedVector(r2l_vec, left);
                }
            }
        }
        return false;
    }

    /**
     @brief Test whether any pair in the set has this left value.
     @param left The left side of the pair to look for.
     */
    bool containsLeft(const LeftType &left) const noexcept
    {
        return m_LeftToRight.contains(left);
    }

    /**
     @brief Test whether any pair in the set has this right value.
     @param right The right side of the pair to look for.
     */
    bool containsRight(const RightType &right) const
    {
        return m_RightToLeft.contains(right);
    }

    /**
     @brief Find all right values that are paired with this left value.
     If nothing is found, you will get an empty array.
     This works well with range-based-for.
     @param left The left side of the pair to look for.
     @return The vector of right values.
     */
    const std::vector<RightType>* findRight(const LeftType &left) const noexcept
    {
        auto l2r_it = m_LeftToRight.find(left);
        if (l2r_it == m_LeftToRight.end())
            return &m_EmptyRightVector;

        return l2r_it->second;
    }

    /**
     @brief Find all left values that are paired with this right value.
     If nothing is found, you will get an empty array.
     This works well with range-based-for.
     @param right The right side of the pair to look for.
     @return The vector of left values.
     */
    const std::vector<LeftType>* findLeft(const RightType &right) const noexcept
    {
        auto r2l_it = m_RightToLeft.find(right);
        if (r2l_it == m_RightToLeft.end())
            return &m_EmptyLeftVector;

        return r2l_it->second;
    }

    /**
     @brief Count the number of left values in the set.
     @return The number of left values in the set.
     */
    int countLeft() const noexcept
    {
        return m_LeftToRight.count();
    }

    /**
     @brief Count the number of right values in the set.
     @return The number of right values in the set.
     */
    int countRight() const noexcept
    {
        return m_RightToLeft.count();
    }

    /**
     @brief Count the number of pairs in the set.
     @return The number of pairs in the set.
     */
    int count() const noexcept
    {
        return m_Count;
    }

    /**
     @brief List all left elements.
     @return A helper object to iterate over left elements using range-based-for.
     */
    UnorderedMapHelper<LeftType, std::vector<RightType> *> allLeft() const noexcept
    {
        return UnorderedMapHelper(&m_LeftToRight);
    }

    /**
     @brief List all right elements.
     @return A helper object to iterate over right elements using range-based-for.
     */
    UnorderedMapHelper<RightType, std::vector<LeftType> *> allRight() const noexcept
    {
        return UnorderedMapHelper(&m_RightToLeft);
    }

    /**
     @brief A range-based-for compatible iterator.
     */
    class Iterator
    {
        /// @cond
      public:
        typename std::unordered_map<LeftType, std::vector<RightType> *>::const_iterator l2r_it;
        typename std::unordered_map<LeftType, std::vector<RightType> *>::const_iterator l2r_it_end;
        typename std::vector<RightType>::const_iterator l2r_vec_it;

        inline Pair operator*() const noexcept
        {
            auto left = l2r_it->first;
            auto right = *l2r_vec_it;
            return Pair(left, right);
        }

        inline bool operator==(const Iterator &other) const noexcept
        {
            return l2r_it == other.LeftIterator;
        }

        inline bool operator!=(const Iterator &other) const noexcept
        {
            return l2r_it != other.l2r_it;
        }

        inline Iterator operator++() noexcept
        {
            l2r_vec_it++;
            if (l2r_vec_it == (*l2r_it).second->cend())
            {
                l2r_it++;
                if (l2r_it != l2r_it_end)
                {
                    l2r_vec_it = (*l2r_it).second->cbegin();
                }
            }
            return *this;
        }
        /// @endcond
    };

    /**
     @brief Required member to get range-based-for.
     @return an Iterator set to the first pair in the set.
     */
    Iterator begin() const noexcept
    {
        Iterator it;
        it.l2r_it = m_LeftToRight.cbegin();
        it.l2r_it_end = m_LeftToRight.cend();
        if (it.l2r_it != it.l2r_it_end)
        {
            auto l2r_vec = it.l2r_it->second;
            it.l2r_vec_it = l2r_vec->cbegin();
        }
        return it;
    }

    /**
     @brief Required member to get range-based-for.
     @return an Iterator set to one after the last pair in the set.
     */
    Iterator end() const noexcept
    {
        Iterator it;
        it.l2r_it = m_LeftToRight.cend();
        return it;
    }
};

// ----------------------------------------------------------------------------

/**
 A one-to-one set of (left, right) pairs. Any left or right value can only be paired with one counterpart.
 @image html binary-relations-one-to-one.png "one-to-one"
 */
template <typename LeftType, typename RightType> class OneToOne
{
    std::unordered_map<LeftType, RightType> m_LeftToRight;
    std::unordered_map<RightType, LeftType> m_RightToLeft;

public:
    /**
    @brief A pair of (left, right) values.
     */
    struct Pair
    {
        LeftType left;
        RightType right;
        Pair(LeftType left, RightType right)
        : left(left), right(right)
        {}
        Pair()
        {}
    };

    /**
     @brief Default constructor.
     */
    OneToOne() noexcept
    {}

    /**
     @brief Insert a pair into the set.
     The rule for one-to-one is that if the rleft value or the ight value are part of an existing pairs in the set, those relations will be erased.
     @param pair The pair to insert.
     */
    void insert(const Pair &pair) noexcept
    {
        insert(pair.left, pair.right);
    }

    /**
     @brief Insert a pair into the set.
     The rule for one-to-one is that if the left value or the right value are part of existing pairs in the set, those relations will be erased.
     @param left The left side of the pair to add.
     @param right The right side of the pair to add.
     */
    void insert(const LeftType &left, const RightType &right) noexcept
    {
        if (!contains(left, right))
        {
            eraseLeft(left);
            eraseRight(right);
            m_RightToLeft[right] = left;
            m_LeftToRight[left] = right;
        }
    }

    /**
     @brief Insert multiple  pairs into the set.
     The rule for one-to-one is that if the left value or the right value are part of existing pairs in the set, those relations will be erased.
     @param pairs The pairs to add.
     */
    void insert(const std::vector<Pair> &pairs) noexcept
    {
        for (auto pair : pairs)
        {
            insert(pair);
        }
    }

    /**
     @brief Erase a pair from the set.
     If there is no matching pair in the set, nothing happens.
     @param pair The pair to erase.
     */
    void erase(const Pair &pair) noexcept
    {
        erase(pair.left, pair.right);
    }

    /**
     @brief Erase a pair from the set.
     If there is no matching pair in the set, nothing happens.
     @param left The left side of the pair to erase.
     @param right The right side of the pair to erase.
     */
    void erase(const LeftType &left, const RightType &right) noexcept
    {
        if (contains(left, right))
        {
            auto l2r_it = m_LeftToRight.find(left);
            auto r2l_it = m_RightToLeft.find(right);

            m_LeftToRight.erase(l2r_it);
            m_RightToLeft.erase(r2l_it);
        }
    }

    /**
     @brief Erase all pairs with the given left value.
     If there is no matching pair in the set, nothing happens.
     @param left The left side of the pairs to erase.
     */
    void eraseLeft(const LeftType &left) noexcept
    {
        auto l2r_it = m_LeftToRight.find(left);
        if (l2r_it != m_LeftToRight.end())
        {
            auto right = l2r_it->second;
            auto r2l_it = m_RightToLeft.find(right);
            m_RightToLeft.erase(r2l_it);
            m_LeftToRight.erase(l2r_it);
        }
    }

    /**
     @brief Erase the pair with the given right value.
     If there is no matching pair in the set, nothing happens.
     @param right The right side of the pairs to erase.
     */
    void eraseRight(const RightType &right) noexcept
    {
        auto r2l_it = m_RightToLeft.find(right);
        if (r2l_it != m_RightToLeft.end())
        {
            auto left = r2l_it->second;
            auto l2r_it = m_LeftToRight.find(left);
            m_LeftToRight.erase(l2r_it);
            m_RightToLeft.erase(r2l_it);
        }
    }

    /**
     @brief Erase multiple pairs from the set.
     @param other The set of pairs to erase.
     */
    void erase(const OneToOne<LeftType, RightType> &other) noexcept
    {
        for (auto pair : other)
        {
            erase( pair);
        }
    }

    /**
     @brief Erase all pairs from the set.
     */
    void clear() noexcept
    {
        m_RightToLeft.clear();
        m_LeftToRight.clear();
    }

    /**
     @brief Test whether a given pair is in the set.
     @param pair The pair to look for.
     */
    bool contains(const Pair &pair) const noexcept
    {
        return contains(pair.left, pair.right);
    }

    /**
     @brief Test whether a given pair is in the set.
     @param left The left side of the pair to look for.
     @param right The right side of the pair to look for.
     */
    bool contains(const LeftType &left, const RightType &right) const noexcept
    {
        auto l2r_it = m_LeftToRight.find(left);
        return l2r_it != m_LeftToRight.end() && l2r_it->second == right;
    }

    /**
     @brief Test whether any pair in the set has this left value.
     @param left The left side of the pair to look for.
     */
    bool containsLeft(const LeftType &left) const noexcept
    {
        auto l2r_it = m_LeftToRight.find(left);
        return l2r_it != m_LeftToRight.end();
    }

    /**
     @brief Test whether any pair in the set has this right value.
     @param right The right side of the pair to look for.
     */
    bool containsRight(const RightType &right) const noexcept
    {
        auto r2l_it = m_RightToLeft.find(right);
        return r2l_it != m_RightToLeft.end();
    }

    /**
     @brief Find the single right value that is paired with this left value.
     If nothing is found, you will get the notFoundValue.
     @param left The left side of the pair to look for.
     @param notFoundValue The value to return if no matching pair is found.
     @return The singular right value.
     */
    RightType findRight(const LeftType &left, const RightType &notFoundValue) const noexcept
    {
        auto l2r_it = m_LeftToRight.find(left);
        return l2r_it != m_LeftToRight.end() ? l2r_it->second : notFoundValue;
    }

    /**
     @brief Find the single left value that is paired with this right value.
     If nothing is found, you will get the notFoundValue.
     @param right The right side of the pair to look for.
     @param notFoundValue The value to return if no matching pair is found.
     @return The singular left value.
     */
    LeftType findLeft(const RightType &right, const LeftType &notFoundValue) const noexcept
    {
        auto r2l_it = m_RightToLeft.find(right);
        return r2l_it != m_RightToLeft.end() ? r2l_it->second : notFoundValue;
    }

    /**
     @brief Count the number of left values in the set.
     @return The number of left values in the set.
     */
    int countLeft() const noexcept
    {
        return (int)m_LeftToRight.size();
    }

    /**
     @brief Count the number of right values in the set.
     @return The number of right values in the set.
     */
    int countRight() const noexcept
    {
        return (int)m_RightToLeft.size();
    }

    /**
     @brief Count the number of pairs in the set.
     @return The number of pairs in the set.
     */
    int count() const noexcept
    {
        return (int)m_LeftToRight.size();
    }

    /**
     @brief List all left elements.
     @return A helper object to iterate over left elements using range-based-for.
     */
    UnorderedMapHelper<LeftType, RightType> allLeft() const noexcept
    {
        return UnorderedMapHelper(&m_LeftToRight);
    }

    /**
     @brief List all right elements.
     @return A helper object to iterate over right elements using range-based-for.
     */
    UnorderedMapHelper<RightType, LeftType> allRight() const noexcept
    {
        return UnorderedMapHelper(&m_RightToLeft);
    }

    /**
     @brief A range-based-for compatible iterator.
     */
    class Iterator
    {
        /// @cond
      public:
        std::unordered_map<LeftType, RightType>::const_iterator l2r_it;

        inline Pair operator*() const noexcept
        {
            return Pair(l2r_it->first, l2r_it->second);
        }

        inline bool operator==(const Iterator &other) const noexcept
        {
            return l2r_it == other.l2r_it;
        }
        inline bool operator!=(const Iterator &other) const noexcept
        {
            return l2r_it != other.l2r_it;
        }
        inline Iterator operator++() noexcept
        {
            l2r_it++;
            return *this;
        }
        /// @endcond
    };

    /**
     @brief Required member to get range-based-for.
     @return an Iterator set to the first pair in the set.
     */
    Iterator begin() const noexcept
    {
        Iterator it;
        it.l2r_it = m_LeftToRight.cbegin();
        return it;
    }

    /**
     @brief Required member to get range-based-for.
     @return an Iterator set to one after the last pair in the set.
     */
    Iterator end() const noexcept
    {
        Iterator it;
        it.l2r_it = m_LeftToRight.cend();
        return it;
    }
};
// ----------------------------------------------------------------------------
} // namespace BinaryRelations
