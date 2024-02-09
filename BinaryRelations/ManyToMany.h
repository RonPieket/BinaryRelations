#pragma once
#include "SortedVector.h"
#include <unordered_map>
#include <vector>

namespace BinaryRelations
{
template <typename LeftType, typename RightType> class ManyToMany
{
    std::unordered_map<LeftType, std::vector<RightType> *> m_LeftToRight;
    std::unordered_map<RightType, std::vector<LeftType> *> m_RightToLeft;
    int m_Count;

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

    ManyToMany() : m_Count(0)
    {
    }

    ManyToMany(const ManyToMany &other) : m_Count(0), m_RightToLeft(other.m_RightToLeft), m_LeftToRight(other.m_LeftToRight)
    {
    }

    void insert(const Pair &pair)
    {
        insert(pair.left, pair.right);
    }

    void insert(const LeftType &left, const RightType &right)
    {
        auto l2r_it = m_LeftToRight.find(left);
        if (l2r_it != m_LeftToRight.end())
        {
            auto l2r_vec = l2r_it->second;
            if (0 != countInSortedVector(l2r_vec, right))
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

    void remove(const Pair &pair)
    {
        remove(pair.left, pair.right);
    }

    void remove(const LeftType &left, const RightType &right)
    {
        auto l2r_it = m_LeftToRight.find(left);
        if (l2r_it != m_LeftToRight.end())
        {
            auto r2l_it = m_RightToLeft.find(right);
            if (r2l_it != m_RightToLeft.end())
            {
                auto l2r_vec = l2r_it->second;
                auto r2l_vec = r2l_it->second;

                removeFromSortedVector(l2r_vec, right);
                removeFromSortedVector(r2l_vec, left);

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

    void removeLeft(const LeftType &left)
    {
        auto l2r_it = m_LeftToRight.constFind(left);
        if (l2r_it != m_LeftToRight.end())
        {
            auto l2r_vec = l2r_it->second;
            for (auto right : l2r_vec)
            {
                auto r2l_it = m_RightToLeft.constFind(right);
                auto r2l_vec = r2l_it->second;
                removeFromSortedVector(*r2l_vec, left);
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

    void removeRight(const RightType &right)
    {
        auto r2l_it = m_RightToLeft.constFind(right);
        if (r2l_it != m_RightToLeft.end())
        {
            auto r2l_vec = r2l_it->second;
            for (auto left : r2l_vec)
            {
                auto l2r_it = m_LeftToRight.constFind(left);
                auto l2r_vec = l2r_it->second;
                removeFromSortedVector(*l2r_vec, right);
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

    void clear()
    {
        m_RightToLeft.clear();
        m_LeftToRight.clear();
    }

    void merge(const ManyToMany<LeftType, RightType> other)
    {
        for (auto pair : other)
        {
            insert(pair);
        }
    }

    bool contains(const Pair &pair) const
    {
        return contains(pair.left, pair.right);
    }

    bool contains(const LeftType &left, const RightType &right) const
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
                    return 0 != countInSortedVector(l2r_vec, right);
                }
                else
                {
                    return 0 != countInSortedVector(r2l_vec, left);
                }
            }
        }
        return false;
    }

    bool containsLeft(const LeftType &left) const
    {
        return m_LeftToRight.contains(left);
    }

    bool containsRight(const RightType &right) const
    {
        return m_RightToLeft.contains(right);
    }

    std::vector<RightType>* findRight(const LeftType &left) const
    {
        auto l2r_it = m_LeftToRight.constFind(left);
        if (l2r_it == m_LeftToRight.end())
            return std::vector<RightType>();

        return l2r_it->second;
    }

    std::vector<LeftType>* findLeft(const RightType &right) const
    {
        auto r2l_it = m_RightToLeft.constFind(right);
        if (r2l_it == m_RightToLeft.end())
            return std::vector<LeftType>();

        return r2l_it->second;
    }

    int countLeft() const
    {
        return m_LeftToRight.count();
    }

    int countRight() const
    {
        return m_RightToLeft.count();
    }

    int count() const
    {
        return m_Count;
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
