/*
 * @Author: Xiyuan Yang   xiyuan_yang@outlook.com
 * @Date: 2025-03-29 21:00:32
 * @LastEditors: Xiyuan Yang   xiyuan_yang@outlook.com
 * @LastEditTime: 2025-04-10 13:18:47
 * @FilePath: /Data_structure/Class_implementation/Set.cpp
 * @Description: Set implementation
 * Do you code and make progress today?
 * Copyright (c) 2025 by Xiyuan Yang, All Rights Reserved. 
 */
/*
implementation for data structure: Set
*/

#include <cstdio>
template<class KEY, class OTHER>
struct set {
    KEY key;
    OTHER other;
};

/**
 * @brief The sequential search for set
 * 
 * @tparam KEY 
 * @tparam OTHER 
 * @param data The set structure
 * @param size The number of elements in the set
 * @param x 
 * @return int the index of the found element (1-based), 0 for finding nothing
 */
template<class KEY, class OTHER>
int seqSearch(set<KEY, OTHER> *data, int size, const KEY &x) {
    data[0].key = x;
    int index;
    for (index = size; x != data[index].key; --index);
    return index;
}


/**
 * @brief for ordered search (assuming it is ascending order), other remain the same.
 * 
 * @tparam KEY 
 * @tparam OTHER 
 * @param data 
 * @param size 
 * @param x 
 * @return int 
 */
template<class KEY, class OTHER>
int seqSearchOrdered(set<KEY, OTHER> *data, int size, const KEY &x) {
    data[0].key = x;
    int index;
    for (index = size; x < data[index].key; --index);
    if (x < data[index].key || index == 0) {
        //not found
        return 0;
    } else {
        return index;
    }

    return 0;
}

/**
 * @brief Binary search for ordered set
 * 
 */
template<class KEY, class OTHER>
int binarySearch(set<KEY, OTHER> *data, int size, const KEY &x) {
    int low = 1;
    int high = size;
    //!remember this set is 1-based
    int mid;
    while (low <= high) {
        mid = low + (high - low) / 2;
        if (x == data[mid].key) {
            return mid;
        }

        //binary, adjusting the min or max
        if (x < data[mid].key) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    //finding nothing
    return 0;
}


/*
TODO chunking search and interpolation search
*/