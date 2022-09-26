#ifndef ALGORITHM
#define ALGORITHM

template<class Iterator, class Comparator>
Iterator find(Iterator first, Iterator end, Comparator comparator)
{
    Iterator current = first;
    while (current != end) 
    {
        if (comparator(*current))
        {
            return current;
        } else 
        {
            current = ++current;
        }
    }
    return Iterator(nullptr);
}

#endif