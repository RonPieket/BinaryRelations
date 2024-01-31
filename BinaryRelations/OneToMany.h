#pragma once

#include <QtCore/QVector>
#include <QtCore/QHash>

#include "SortedVector.h"

namespace BinaryRelations
{
    template< typename LeftType, typename RightType >
    class OneToMany
    {
    public:
        struct Pair
        {
            LeftType left;
            RightType right;
            Pair(LeftType left, RightType right) : left(left), right(right) {}
            Pair() {}
        };

        OneToMany()
        {}

        OneToMany( const OneToMany& other )
            : m_RightToLeft( other.m_RightToLeft )
            , m_LeftToRight( other.m_LeftToRight )
        {}

        void insert( const Pair& pair )
        {
            insert( pair.left, pair.right );
        }

        void insert( const LeftType& left, const RightType& right )
        {
            auto left_it = m_RightToLeft.find( right );
            if( left_it != m_RightToLeft.end() )
            {
                if( *left_it == left )
                {
                    // We already have this pair - do nothing
                    return;
                }
                // Remove old relation
                remove( *left_it, right );
            }

            QVector< RightType >& right_vector = m_LeftToRight[ left ]; // Will insert if it isn't already there.
            bool was_inserted = insertIntoSortedVector( &right_vector, right );
            Q_ASSERT( was_inserted );   // Table corrupt

            m_RightToLeft.insert( right, left );
        }

        void remmove( const Pair& pair )
        {
            remove( pair.left, pair.right );
        }

        void remove( const LeftType& left, const RightType& right )
        {
            auto left_it = m_RightToLeft.find( right );
            if( left_it == m_RightToLeft.end() )
            {
                // (any,right) not in the set - do nothing
                return;
            }
            if( *left_it != left )
            {
                // (left,right) is not in the set - do nothing
                return;
            }

            QVector< RightType >& right_vector = m_LeftToRight[ left ];
            int index = findInSortedVector(right_vector, right);
            if( index != -1 )
            {
                right_vector.remove( index );
                if( right_vector.size() == 0 )
                {
                    m_LeftToRight.remove( left );
                }
                m_RightToLeft.remove( right );
            }
        }

        void removeLeft( const LeftType& left )
        {
            QVector< RightType > right_vector = m_LeftToRight.value( left );
            for ( RightType right : right_vector )
            {
                m_RightToLeft.remove( right );
            }
            m_LeftToRight.remove( left );
        }

        void removeRight( const RightType& right )
        {
            auto left_it = m_RightToLeft.find( right );
            if( left_it != m_RightToLeft.end() )
            {
                remove( left_it.value(), right );
            }
        }

        void clear()
        {
            m_RightToLeft.clear();
            m_LeftToRight.clear();
        }

        void unite(const OneToMany<LeftType, RightType> other)
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
            return left_it != m_RightToLeft.constEnd() && *left_it == left;
        }

        bool containsLeft( const LeftType& left) const
        {
            return m_LeftToRight.contains(left);
        }

        bool containsRight( const RightType& right) const
        {
            return m_RightToLeft.contains(right);
        }

        QVector< RightType > findRight( const LeftType& left ) const
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
            typename QHash< LeftType, QVector< RightType > >::const_key_value_iterator LeftIterator;
            typename QHash< LeftType, QVector< RightType > >::const_key_value_iterator LeftIteratorEnd;
            typename QVector< RightType >::const_iterator RightIterator;

            inline Pair operator*() const noexcept
            {
                return Pair ((*LeftIterator).first, *RightIterator);
            }

            inline bool operator==(const Iterator &other) const noexcept { return LeftIterator == other.LeftIterator; }
            inline bool operator!=(const Iterator &other) const noexcept { return LeftIterator != other.LeftIterator; }

            inline Iterator operator++() noexcept
            {
                RightIterator++;
                if (RightIterator == (*LeftIterator).second.constEnd())
                {
                    LeftIterator++;
                    if (LeftIterator != LeftIteratorEnd)
                    {
                        RightIterator = (*LeftIterator).second.constBegin();
                    }
                }
                return *this;
            }
        };

        Iterator begin()
        {
            Iterator it;
            it.LeftIterator = m_LeftToRight.constKeyValueBegin();
            it.LeftIteratorEnd = m_LeftToRight.constKeyValueEnd();
            if(it.LeftIterator != it.LeftIteratorEnd)
            {
                it.RightIterator = (*it.LeftIterator).second.constBegin();
            }
            return it;
        }

        Iterator end()
        {
            Iterator it;
            it.LeftIterator = m_LeftToRight.constKeyValueEnd();
            return it;
        }

    private:
        QHash< RightType, LeftType >            m_RightToLeft;
        QHash< LeftType, QVector< RightType > > m_LeftToRight;
    };
}
