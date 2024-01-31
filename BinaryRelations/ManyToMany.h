#pragma once

#include <QtCore/QVector>
#include <QtCore/QHash>

#include "SortedVector.h"

namespace BinaryRelations
{
    template< typename LeftType, typename RightType >
    class ManyToMany
    {
    public:

        struct Pair
        {
            LeftType left;
            RightType right;
            Pair(LeftType left, RightType right) : left(left), right(right) {}
            Pair() {}
        };

        ManyToMany()
        {}

        ManyToMany( const ManyToMany& other )
            : m_RightToLeft( other.m_RightToLeft )
            , m_LeftToRight( other.m_LeftToRight )
        {}

        void insert( const Pair& pair )
        {
            insert( pair.left, pair.right );
        }

        void insert( const LeftType& left, const RightType& right )
        {
            QVector< RightType >& right_vector = m_LeftToRight[ left ]; // Will insert if it isn't already there.
            bool was_inserted = insertIntoSortedVector( &right_vector, right );
            if( was_inserted )
            {
                QVector< LeftType >& left_vector = m_RightToLeft[ right ]; // Will insert if it isn't already there.
                was_inserted = insertIntoSortedVector( &left_vector, left );
                Q_ASSERT( was_inserted );   // Table corrupt
            }
        }

        void remove( const Pair& pair )
        {
            remove( pair.left, pair.right );
        }

        void remove( const LeftType& left, const RightType& right )
        {
            auto right_vector_it = m_LeftToRight.find( left );
            if( right_vector_it != m_LeftToRight.end() )
            {
                QVector< RightType >& right_vector = right_vector_it.value();
                bool was_removed = removeFromSortedVector( &right_vector, right );
                if( was_removed )
                {
                    auto left_vector_it = m_RightToLeft.find( right );
                    Q_ASSERT( left_vector_it != m_RightToLeft.end() ); // Table corrupt

                    QVector< LeftType >& left_vector = left_vector_it.value();
                    was_removed = removeFromSortedVector( &left_vector, left );
                    Q_ASSERT( was_removed );    // Table corrupt

                    if( right_vector.count() == 0 )
                    {
                        m_LeftToRight.remove( left );
                    }
                    if( left_vector.count() == 0 )
                    {
                        m_RightToLeft.remove( right );
                    }
                }
            }
        }

        void removeLeft( const LeftType& left )
        {
            auto right_vector_it = m_LeftToRight.constFind( left );
            if( right_vector_it != m_LeftToRight.constEnd() )
            {
                const QVector< RightType >& right_vector = right_vector_it.value();
                for ( RightType right : right_vector )
                {
                    auto left_vector_it = m_RightToLeft.find( right );
                    Q_ASSERT( left_vector_it != m_RightToLeft.end() );  // Table corrupt

                    QVector< LeftType >& left_vector = left_vector_it.value();
                    bool was_removed = removeFromSortedVector( &left_vector, left );
                    Q_ASSERT( was_removed );    // Table corrupt

                    if( left_vector.size() == 0 )
                    {
                        m_RightToLeft.remove( right );
                    }
                }
                m_LeftToRight.remove( left );
            }
        }

        void removeRight( const RightType& right )
        {
            auto left_vector_it = m_RightToLeft.constFind( right );
            if( left_vector_it != m_RightToLeft.constEnd() )
            {
                const QVector< LeftType >& left_vector = left_vector_it.value();
                for ( LeftType left : left_vector )
                {
                    auto right_vector_it = m_LeftToRight.find( left );
                    Q_ASSERT( right_vector_it != m_LeftToRight.end() ); // Table corrupt

                    QVector< RightType >& right_vector = right_vector_it.value();
                    bool was_removed = removeFromSortedVector( &right_vector, right );
                    Q_ASSERT( was_removed );    // Table corrupt

                    if( right_vector.size() == 0 )
                    {
                        m_LeftToRight.remove( left );
                    }
                }
                m_RightToLeft.remove( right );
            }
        }

        void clear()
        {
            m_RightToLeft.clear();
            m_LeftToRight.clear();
        }

        void unite(const ManyToMany<LeftType, RightType> other)
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
            auto right_vector_it = m_LeftToRight.constFind( left );
            if( right_vector_it != m_LeftToRight.end() )
            {
                auto left_vector_it = m_RightToLeft.constFind( right );
                if( left_vector_it != m_RightToLeft.end() )
                {
                    const QVector< RightType >& right_vector = right_vector_it.value();
                    const QVector< LeftType >& left_vector = left_vector_it.value();
                    if( right_vector.count() < left_vector.count() )
                    {
                        return 0 != countInSortedVector( right_vector, right );
                    }
                    else
                    {
                        return 0 != countInSortedVector( left_vector, left );
                    }
                }
            }
            return false;
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

        QVector< LeftType > findLeft( const RightType& right ) const
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

        // ----------

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
        QHash< RightType, QVector< LeftType > > m_RightToLeft;
        QHash< LeftType, QVector< RightType > > m_LeftToRight;
    };
}
