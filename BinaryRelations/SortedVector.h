#pragma once

#include <algorithm>

#include <vector>

namespace BinaryRelations
{
// -------- Manipulate vector with unique sorted elements --------

template <typename T> int countInSortedVector(const std::vector<T> *vector, const T &value)
{
    typename std::vector<T>::const_iterator it = std::lower_bound(vector->cbegin(), vector->cend(), value);
    return it != vector->cend() && *it == value ? 1 : 0;
}

template <typename T>
typename std::vector<T>::const_iterator findInSortedVector(const std::vector<T> *vector, const T &value)
{
    typename std::vector<T>::const_iterator it = std::lower_bound(vector->cbegin(), vector->cend(), value);
    if (it != vector->cend() && *it == value)
        return it;
    else
        return vector->cend();
}

template <typename T> int insertIntoSortedVector(std::vector<T> *vector, const T &value)
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

template <typename T> int removeFromSortedVector(std::vector<T> *vector, const T &value)
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
void mergeSortedVectors(std::vector<T> *vectorInA, std::vector<T> *vectorInB, std::vector<T> *vectorOut)
{
    typename std::vector<T>::const_iterator itInA = vectorInA->cbegin();
    typename std::vector<T>::const_iterator itInB = vectorInB->cbegin();

    vectorOut->clear();
    vectorOut->reserve(vectorInA->count() + vectorInB->count());

    while (itInA != vectorInA->cend() && itInB != vectorInB->cend())
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

    while (itInA != vectorInA->cend())
        vectorOut->append(*itInA++);

    while (itInB != vectorInB->cend())
        vectorOut->append(*itInB++);
}
} // namespace BinaryRelations
