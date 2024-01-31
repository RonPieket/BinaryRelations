#pragma once

#include <QtCore/QVector>
#include <QtCore/QHash>

#include "SortedVector.h"

namespace BinaryRelations
{
    template< typename LeftType, typename RightType >
    class OneToOne
    {
    public:
        struct Pair
        {
            LeftType left;
            RightType right;
            Pair(LeftType left, RightType right) : left(left), right(right) {}
            Pair() {}
        };

        OneToOne()
        {}

        OneToOne( const OneToOne& other )
            : m_RightToLeft( other.m_RightToLeft )
            , m_LeftToRight( other.m_LeftToRight )
        {}

        void insert( const Pair& pair )
        {
            insert( pair.left, pair.right );
        }

        void insert( const LeftType& left, const RightType& right )
        {
            if( !contains( left, right ) )
            {
                removeLeft( left );
                removeRight( right );
                m_RightToLeft.insert( right, left );
                m_LeftToRight.insert( left, right );
            }
        }

        void remove( const Pair& pair )
        {
            remove( pair.left, pair.right );
        }

        void remove( const LeftType& left, const RightType& right )
        {
            if( contains( left, right ) )
            {
                m_RightToLeft.remove( right );
                m_LeftToRight.remove( left );
            }
        }

        void removeLeft( const LeftType& left )
        {
            auto right_it = m_LeftToRight.find( left );
            if( right_it != m_LeftToRight.end() )
            {
                m_RightToLeft.remove( right_it.value() );
                m_LeftToRight.remove( left );
            }
        }

        void removeRight( const RightType& right )
        {
            auto left_it = m_RightToLeft.find( right );
            if( left_it != m_RightToLeft.end() )
            {
                m_LeftToRight.remove( left_it.value() );
                m_RightToLeft.remove( right );
            }
        }

        void clear()
        {
            m_RightToLeft.clear();
            m_LeftToRight.clear();
        }

        void unite(const OneToOne<LeftType, RightType> other)
        {
            for (auto pair : other)
            {
                insert( pair.left, pair.right);
            }
        }

        bool contains( const Pair& pair ) const
        {
            return contains( pair.left, pair.right );
        }

        bool contains( const LeftType& left, const RightType& right ) const
        {
            auto left_it = m_RightToLeft.find( right );
            return left_it != m_RightToLeft.end() && left_it.value() == left;
        }

        bool containsLeft( const LeftType& left) const
        {
            return m_LeftToRight.contains(left);
        }

        bool containsRight( const RightType& right) const
        {
            return m_RightToLeft.contains(right);
        }

        RightType findRight( const LeftType& left ) const
        {
            return m_LeftToRight.value( left );
        }

        LeftType findLeft( const RightType& right ) const
        {
            return m_RightToLeft.value( right );
        }

        int countLeft() const
        {
            return m_LeftToRight.count();
        }

        int countRight() const
        {
            return m_RightToLeft.count();
        }

        // -------

    public:

        class Iterator
        {
        public:
            typename QHash< LeftType, RightType >::const_key_value_iterator it;

            inline Pair operator*() const noexcept
            {
                return Pair((*it).first, (*it).second);
            }

            inline bool operator==(const Iterator &o) const noexcept { return it == o.it; }
            inline bool operator!=(const Iterator &o) const noexcept { return it != o.it; }
            inline Iterator operator++() noexcept
            {
                it++;
                return *this;
            }
        };

        Iterator begin()
        {
            Iterator it;
            it.it = m_LeftToRight.constKeyValueBegin();
            return it;
        }

        Iterator end()
        {
            Iterator it;
            it.it = m_LeftToRight.constKeyValueEnd();
            return it;
        }

    private:
        QHash< RightType, LeftType >            m_RightToLeft;
        QHash< LeftType, RightType >            m_LeftToRight;
    };
}
