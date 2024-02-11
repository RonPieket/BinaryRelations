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
void insertIntoSortedVector(std::vector<T> *sourceVector, std::vector<T> *insertVector, std::vector<T> *outVector)
{
    typename std::vector<T>::const_iterator sourceIt = sourceVector->cbegin();
    typename std::vector<T>::const_iterator insertIt = insertVector->cbegin();

    outVector->clear();
    outVector->reserve(sourceVector->count() + insertVector->count());

    while (sourceIt != sourceVector->cend() && insertIt != insertVector->cend())
    {
        if (*sourceIt == *insertIt)
        {
            outVector->append(*sourceIt++);
            insertIt++;
        }
        else if (*sourceIt < *insertIt)
            outVector->append(*sourceIt++);
        else
            outVector->append(*insertIt++);
    }

    while (sourceIt != sourceVector->cend())
        outVector->append(*sourceIt++);

    while (insertIt != insertVector->cend())
        outVector->append(*insertIt++);
}

template <typename T>
void removeFromSortedVector(std::vector<T> *sourceVector, std::vector<T> *removeVector, std::vector<T> *outVector)
{
    typename std::vector<T>::const_iterator sourceIt = sourceVector->cbegin();
    typename std::vector<T>::const_iterator removeIt = removeVector->cbegin();

    outVector->clear();
    outVector->reserve(std::max(sourceVector->count(), removeVector->count()));

    while (sourceIt != sourceVector->cend() && removeIt != removeVector->cend())
    {
        if (*sourceIt == *removeIt)
        {
            sourceIt++;
            removeIt++;
        }
        else if (*sourceIt < *removeIt)
            outVector->append(*sourceIt++);
        else
            outVector->append(*removeIt++);
    }

    while (sourceIt != sourceVector->cend())
        outVector->append(*sourceIt++);
}
} // namespace BinaryRelations
