#pragma once
#include "SortedVector.h"
#include <unordered_map>
#include <vector>

namespace BinaryRelations
{
template <typename LeftType, typename RightType> class OneToOne
{
    std::unordered_map<LeftType, RightType> m_LeftToRight;
    std::unordered_map<RightType, LeftType> m_RightToLeft;

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

    OneToOne()
    {
    }

    OneToOne(const OneToOne &other) : m_RightToLeft(other.m_RightToLeft), m_LeftToRight(other.m_LeftToRight)
    {
    }

    void insert(const Pair &pair)
    {
        insert(pair.left, pair.right);
    }

    void insert(const LeftType &left, const RightType &right)
    {
        if (!contains(left, right))
        {
            removeLeft(left);
            removeRight(right);
            m_RightToLeft[right] = left;
            m_LeftToRight[left] = right;
        }
    }

    void remove(const Pair &pair)
    {
        remove(pair.left, pair.right);
    }

    void remove(const LeftType &left, const RightType &right)
    {
        if (contains(left, right))
        {
            auto l2r_it = m_LeftToRight.find(left);
            auto r2l_it = m_RightToLeft.find(right);

            m_LeftToRight.erase(l2r_it);
            m_RightToLeft.erase(r2l_it);
        }
    }

    void removeLeft(const LeftType &left)
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

    void removeRight(const RightType &right)
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

    void insert(const OneToMany<LeftType, RightType> other)
    {
        for (auto pair : other)
        {
            insert(pair);
        }
    }

    void remove(const OneToMany<LeftType, RightType> other)
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
        auto l2r_it = m_LeftToRight.find(left);
        return l2r_it != m_LeftToRight.end() && l2r_it->second == right;
    }

    bool containsLeft(const LeftType &left) const
    {
        auto l2r_it = m_LeftToRight.find(left);
        return l2r_it != m_LeftToRight.end();
    }

    bool containsRight(const RightType &right) const
    {
        auto r2l_it = m_RightToLeft.find(right);
        return r2l_it != m_RightToLeft.end();
    }

    RightType findRight(const LeftType &left, const RightType &defaultValue) const
    {
        auto l2r_it = m_LeftToRight.find(left);
        return l2r_it != m_LeftToRight.end() ? l2r_it->second : defaultValue;
    }

    LeftType findLeft(const RightType &right, const LeftType &defaultValue) const
    {
        auto r2l_it = m_RightToLeft.find(right);
        return r2l_it != m_RightToLeft.end() ? r2l_it->second : defaultValue;
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
        return (int)m_LeftToRight.size();
    }

    class Iterator
    {
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
    };

    Iterator begin() const
    {
        Iterator it;
        it.l2r_it = m_LeftToRight.cbegin();
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
