#pragma once

#include <algorithm>

#include <QtCore/QVector>

namespace BinaryRelations
{
    // -------- Manipulate vector with unique sorted elements --------

    template< typename T >
    int countInSortedVector( const QVector< T >& vector, const T& value )
    {
        typename QVector< T >::const_iterator it = std::lower_bound( vector.constBegin(), vector.constEnd(), value );
        return it != vector.constEnd() && *it == value ? 1 : 0;
    }

    template< typename T >
    int findInSortedVector( const QVector< T >& vector, const T& value )
    {
        typename QVector< T >::const_iterator it = std::lower_bound( vector.constBegin(), vector.constEnd(), value );
        if( it != vector.constEnd() && *it == value )
        {
            return it - vector.constBegin();
        }
        else
        {
            return -1;
        }
    }

    template< typename T >
    int insertIntoSortedVector( QVector< T >* vector, const T& value )
    {
        typename QVector< T >::iterator it = std::lower_bound( vector->begin(), vector->end(), value );
        if( it != vector->end() && *it == value )
        {
            // It's already there
            return 0;
        }
        else
        {
            vector->insert( it, value );
            return 1;
        }
    }

    template<typename T>
    int removeFromSortedVector(QVector<T> *vector, const T &value)
    {
        typename QVector<T>::iterator it = std::lower_bound(vector->begin(), vector->end(), value);
        if (it != vector->end() && *it == value) {
            vector->erase(it);
            return 1;
        } else {
            return 0;
        }
    }

    template<typename T>
    void mergeSortedVectors(QVector<T> *vectorInA, QVector<T> *vectorInB, QVector<T> *vectorOut)
    {
        typename QVector<T>::const_iterator itInA = vectorInA->constBegin();
        typename QVector<T>::const_iterator itInB = vectorInB->constBegin();

        vectorOut->clear();
        vectorOut->reserve(vectorInA->count() + vectorInB->count());

        while (itInA != vectorInA->constEnd() && itInB != vectorInB->constEnd())
        {
            if (*itInA == *itInB)
            {
                vectorOut->append(*itInA++);
                itInB++;
            }
            else if (*itInA < *itInB)
                vectorOut->append(*itInA++);
            else
                vectorOut->append(*itInB++);
        }

        while (itInA != vectorInA->constEnd())
            vectorOut->append(*itInA++);

        while (itInB != vectorInB->constEnd())
            vectorOut->append(*itInB++);
    }

    // -------- Manipulate vector with non-unique sorted elements --------

    template< typename T >
    int countInSortedMultiVector( const QVector< T >& vector, const T& value )
    {
        std::pair< typename QVector< T >::const_iterator, typename QVector< T >::const_iterator > it = std::equal_range( vector.constBegin(), vector.constEnd(), value );
        return it.second - it.first;
    }

    template< typename T >
    int findInSortedMultiVector( const QVector< T >& vector, const T& value )
    {
        return findInSortedVector( vector, value );
    }

    template< typename T >
    int insertIntoSortedMultiVector( QVector< T >* vector, const T& value )
    {
        typename QVector< T >::iterator it = std::upper_bound( vector->begin(), vector->end(), value );
        vector->insert( it, value );
        return 1;
    }

    template< typename T >
    int removeAllFromSortedMultiVector( QVector< T >* vector, const T& value )
    {
        typename std::pair< typename QVector< T >::const_iterator, typename QVector< T >::const_iterator > it = std::equal_range( vector->constData(), vector->constEnd(), value );
        vector->erase( it.first, it.second );
        return it.second - it.first;
    }

    template< typename T >
    int removeOneFromSortedMultiVector( QVector< T >* vector, const T& value )
    {
        return removeFromSortedVector( vector, value );
    }


    template<typename T>
    void mergeSortedMultiVectors(QVector<T> *vectorInA, QVector<T> *vectorInB, QVector<T> *vectorOut)
    {
        typename QVector<T>::const_iterator itInA = vectorInA->constBegin();
        typename QVector<T>::const_iterator itInB = vectorInB->constBegin();

        vectorOut->clear();
        vectorOut->reserve(vectorInA->count() + vectorInB->count());

        while (itInA != vectorInA->constEnd() && itInB != vectorInB->constEnd())
        {
            if (*itInA < *itInB)
                vectorOut->append(*itInA++);
            else
                vectorOut->append(*itInB++);
        }

        while (itInA != vectorInA->constEnd())
            vectorOut->append(*itInA++);

        while (itInB != vectorInB->constEnd())
            vectorOut->append(*itInB++);
    }
}
