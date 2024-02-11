#pragma once
#include "SortedVector.h"
#include <unordered_map>
#include <vector>

namespace BinaryRelations
{
template <typename LeftType, typename RightType> class OneToMany
{
    std::unordered_map<LeftType, std::vector<RightType> *> m_LeftToRight;
    std::unordered_map<RightType, LeftType> m_RightToLeft;
    std::vector<RightType> m_EmptyRightVector;

public:
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

    OneToMany()
    {
    }

    void insert(const Pair &pair)
    {
        insert(pair.left, pair.right);
    }

    void insert(const LeftType &left, const RightType &right)
    {
        auto r2l_it = m_RightToLeft.find(right);
        if (r2l_it != m_RightToLeft.end())
        {
            if (r2l_it->second == left)
                return; // We already have this pair - do nothing

            remove(r2l_it->second, right); // Remove old relation
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

    void insert(const OneToMany<LeftType, RightType> &other)
    {
        for (auto pair : other)
        {
            insert(pair);
        }
    }

    void remove(const Pair &pair)
    {
        remove(pair.left, pair.right);
    }

    void remove(const LeftType &left, const RightType &right)
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

    void removeLeft(const LeftType &left)
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

    void removeRight(const RightType &right)
    {
        auto r2l_it = m_RightToLeft.find(right);
        if (r2l_it == m_RightToLeft.end())
            return;
        remove(r2l_it->second, right);
    }

    void remove(const OneToMany<LeftType, RightType> &other)
    {
        for (auto pair : other)
        {
            remove(pair);
        }
    }

    void clear()
    {
        m_RightToLeft.clear();
        m_LeftToRight.clear();
    }

    bool contains(const Pair &pair) const
    {
        return contains(pair.left, pair.right);
    }

    bool contains(const LeftType &left, const RightType &right) const
    {
        auto r2l_it = m_RightToLeft.find(right);
        return r2l_it != m_RightToLeft.cend() && r2l_it->second == left;
    }

    bool containsLeft(const LeftType &left) const
    {
        return m_LeftToRight.contains(left);
    }

    bool containsRight(const RightType &right) const
    {
        return m_RightToLeft.contains(right);
    }

    const std::vector<RightType>* findRight(const LeftType &left) const
    {
        auto l2r_it = m_LeftToRight.find(left);
        if (l2r_it == m_LeftToRight.end())
            return &m_EmptyRightVector;

        return l2r_it->second;
    }

    LeftType findLeft(const RightType &right, const LeftType &notFoundValue) const
    {
        auto r2l_it = m_RightToLeft.find(right);
        if (r2l_it == m_RightToLeft.end())
            return notFoundValue;

        return r2l_it->second;
    }

    int countLeft() const
    {
        return (int)m_LeftToRight.size();
    }

    int countRight() const
    {
        return (int)m_RightToLeft.size();
    }

    int count() const
    {
        return (int)m_RightToLeft.size();
    }

    class Iterator
    {
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
    };

    Iterator begin() const
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

    Iterator end() const
    {
        Iterator it;
        it.l2r_it = m_LeftToRight.cend();
        return it;
    }
};
} // namespace BinaryRelations
