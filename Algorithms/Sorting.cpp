// For all kinds of sorting algorithms
#include <cstdlib>
#include <iostream>
#include <queue>
#include <random>

template<class KEY, class OTHER>
struct set {
    KEY key = KEY();
    OTHER other = OTHER();
};

// Define SortFunction as a function pointer type
template<class Key, class Other>
using SortFunction = void (*)(set<Key, Other> *, int);

// Simple insertion sort
template<class Key, class Other>
void simpleInsertSort(set<Key, Other> *a, int size) {
    int k;
    set<Key, Other> tmp;

    // Start sorting
    for (int i = 1; i < size; i++) {
        tmp = a[i];
        for (k = i - 1; k >= 0 && tmp.key < a[k].key; --k) {
            a[k + 1] = a[k];
        }
        a[k + 1] = tmp;
    }
}

template<class Key, class Other>
void shellSort(set<Key, Other> *a, int size) {
    int step, i, j;
    set<Key, Other> tmp;

    for (step = size / 2; step > 0; step /= 2) {
        // shell array, finally is 1
        // do insert sort with stepsize of step
        for (i = step; i < size; i++) {
            tmp = a[i];
            for (j = i - step; j >= 0 && a[j].key > tmp.key; j -= step) {
                a[j + step] = a[j];
            }
            a[j + step] = tmp;
        }
    }
}

template<class Key, class Other>
void heapSort(set<Key, Other> *a, int size) {
    // Use a custom comparator for a min-heap
    auto cmp = [](const set<Key, Other> &lhs, const set<Key, Other> &rhs) {
        return lhs.key > rhs.key;// Min-heap: smaller key has higher priority
    };

    std::priority_queue<set<Key, Other>, std::vector<set<Key, Other>>, decltype(cmp)> pq(cmp);

    // Build the heap
    for (int i = 0; i < size; i++) {
        pq.push(a[i]);
    }

    // Extract elements from the heap in sorted order
    for (int i = 0; i < size; i++) {
        a[i] = pq.top();
        pq.pop();
    }
}

template<class Key, class Other>
void OwnHeapSort(set<Key, Other> *a, int size) {
    for (int i = size / 2 - 1; i >= 0; i--) {
        // build for all nodes
        percolateDown(a, i, size);
    }

    // dequeue
    /*
    i is the size of the current heap
    std::swap(a[0], a[i]) means pop the first element of the heap (swap with the ith element at the back)
    then use percolatedown to make the heap sorted again.(Use for a[0] only)
    */
    for (int i = size - 1; i > 0; i--) {
        std::swap(a[0], a[i]);
        percolateDown(a, 0, i);
    }
}

/**
 * @brief percolate down for the heap sort
 * 
 * @tparam Key 
 * @tparam Other 
 * @param a the array to be sorted
 * @param hole the starting position of percolate down
 * @param size the size of the heap
 */
template<class Key, class Other>
void percolateDown(set<Key, Other> *a, int hole, int size) {
    auto tmp = a[hole];
    int child = 0;
    while (hole * 2 + 1 < size) {
        // !attention! it is 0-based here
        child = hole * 2 + 1;
        // left child
        if (child != size - 1 && a[child + 1].key > a[child].key) {
            // choose the right child
            ++child;
        }

        if (a[child].key > tmp.key) {
            a[hole] = a[child];
            hole = child;
        } else {
            break;
        }
    }
    a[hole] = tmp;
}

template<class Key, class Other>
int partition(set<Key, Other> *a, int low, int high) {
    set<Key, Other> pivot = a[low];
    int i = low + 1;
    int j = high;

    while (true) {
        while (i <= j && a[i].key <= pivot.key) {
            i++;
        }

        while (i <= j && a[j].key > pivot.key) {
            j--;
        }

        if (i >= j) {
            break;
        }

        std::swap(a[i], a[j]);
    }
    std::swap(a[low], a[j]);
    return j;
}

template<class Key, class Other>
int randomizedPartition(set<Key, Other> *a, int low, int high) {
    // Generate a random index between low and high
    int randomIndex = low + rand() % (high - low + 1);

    // Swap the randomly chosen pivot with the first element
    std::swap(a[low], a[randomIndex]);

    // Proceed with the normal partition logic
    set<Key, Other> pivot = a[low];
    int i = low + 1;
    int j = high;

    while (true) {
        while (i <= j && a[i].key <= pivot.key) {
            i++;
        }

        while (i <= j && a[j].key > pivot.key) {
            j--;
        }

        if (i >= j) {
            break;
        }

        std::swap(a[i], a[j]);
    }
    std::swap(a[low], a[j]);
    return j;
}

template<class Key, class Other>
void QuickSort(set<Key, Other> *a, int size) {
    Quicksort(a, 0, size - 1);
}

template<class Key, class Other>
void Quicksort(set<Key, Other> *a, int low, int high) {

    if (low >= high) {
        return;
        // the recursion end
    }
    // make the partition
    int mid = partition(a, low, high);

    // recursion
    Quicksort(a, low, mid - 1);
    Quicksort(a, mid + 1, high);
}

template<class Key, class Other>
void randomizedQuicksort(set<Key, Other> *a, int low, int high) {
    if (low >= high) {
        return;// The recursion ends
    }

    // Make the partition using randomizedPartition
    int mid = randomizedPartition(a, low, high);

    // Recursion
    randomizedQuicksort(a, low, mid - 1);
    randomizedQuicksort(a, mid + 1, high);
}

template<class Key, class Other>
void RandomizedQuickSort(set<Key, Other> *a, int size) {
    randomizedQuicksort(a, 0, size - 1);
}

template<class Key, class Other>
void BubbleSort(set<Key, Other> *a, int size) {
    bool flag = true;
    for (int i = 1; i < size && flag; i++) {
        flag = false;
        for (int j = 0; j < size - i; j++) {
            if (a[j + 1].key < a[j].key) {
                // swap!
                std::swap(a[j + 1], a[j]);
                flag = true;
            }
        }
    }
}

template<class Key, class Other>
void selectSort(set<Key, Other> *a, int size) {
    for (int i = 0; i < size - 1; i++) {
        int min = i;
        for (int j = i + 1; j < size; j++) {
            // the unsorted array
            if (a[j].key < a[min].key) {
                // find the minimum element
                min = j;
            }
        }
        std::swap(a[i], a[min]);
    }
}

/**
 * @brief merge the two sorted array, one is [left, mid -1], and the other is [mid, right]
 * 
 * @tparam Key 
 * @tparam Other 
 */
template<class Key, class Other>
void Merge(set<Key, Other> *a, int left, int mid, int right) {
    set<Key, Other> *tmp = new set<Key, Other>[right - left + 1];

    int i = left, j = mid, k = 0;

    while (i < mid && j <= right) {
        // the two sorted list doesn't end
        if (a[i].key < a[j].key) {
            tmp[k++] = a[i++];
        } else {
            tmp[k++] = a[j++];
        }
    }

    while (i < mid) {
        tmp[k++] = a[i++];
    }

    while (j <= right) {
        tmp[k++] = a[j++];
    }

    // move the sorted array to the original list
    for (i = 0, k = left; k <= right; i++, k++) {
        a[k] = tmp[i];
    }

    delete[] tmp;
}

template<class Key, class Other>
void MergeSort_(set<Key, Other> *a, int left, int right) {
    int mid = (left + right) / 2;
    if (left == right) {
        return;
    }

    MergeSort_(a, left, mid);
    MergeSort_(a, mid + 1, right);
    Merge(a, left, mid + 1, right);
}

template<class Key, class Other>
void MergeSort(set<Key, Other> *a, int size) {
    MergeSort_(a, 0, size - 1);
}

//implementation of bucket sort
template<class Other>
struct node {
    set<int, Other> data;
    node *next;

    node() {
        next = nullptr;
    }

    node(const set<int, Other> &d) {
        data = d;
        next = nullptr;
    }
};

template<class Other>
void BucketSort(node<Other> *&p) {
    node<Other> *bucket[10], *last[10], *tail;
    int max = 0, len = 0;

    // find the maximum key
    for (tail = p; tail != nullptr; tail = tail->next) {
        max = std::max(tail->data.key, max);
    }

    // find the length
    if (max == 0) {
        len = 0;
    } else {
        while (max > 0) {
            ++len;
            max /= 10;
        }
    }

    // starting the bucket sort
    int base = 1;
    for (int i = 1; i < len; i++, base *= 10) {
        // clear all the bucket
        for (int j = 0; j < 9; j++) {
            bucket[j] = last[j] = nullptr;
        }

        // allocate bucket for every element
        while (p != nullptr) {
            int k = ((p->data.key) / base) % 10;

            // add to different bucket!
            if (bucket[k] == nullptr) {
                // remains empty
                bucket[k] = last[k] = p;
            } else {
                last[k]->next = p;
                last[k] = last[k]->next;
            }
            p = p->next;
        }

        p = nullptr;
        // reconstruct, now p is the beginning and tail is the tail
        for (int j = 0; j < 9; j++) {
            if (bucket[j] == nullptr) {
                continue;
            }

            if (p == nullptr) {
                p = bucket[j];
            } else {
                tail->next = bucket[j];
            }

            tail = last[j];
        }

        tail->next = nullptr;
    }
}

int generate_random() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100000);
    return dis(gen);
}

// Helper function to generate a random string of given length
std::string generate_random_string(int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const int charset_size = sizeof(charset) - 1;

    std::string result;
    result.reserve(length);

    for (int i = 0; i < length; i++) {
        result += charset[rand() % charset_size];
    }

    return result;
}

// Test function that accepts a function pointer
template<class Key, class Other>
bool test_sort(SortFunction<Key, Other> sortFunc) {
    // Generate a random array of size 1000
    const int size = 1000;
    set<Key, Other> arr[size];

    // Fill the array with random data
    for (int i = 0; i < size; i++) {
        arr[i].key = generate_random();           // Random integer key
        arr[i].other = generate_random_string(10);// Random string of length 10
    }

    // Sort the array using the provided sorting function
    sortFunc(arr, size);

    // Check if the array is sorted in ascending order
    for (int i = 1; i < size; i++) {
        if (arr[i].key < arr[i - 1].key) {
            return false;// Array is not sorted
        }
    }

    return true;// Array is sorted
}


int main() {
    SortFunction<int, std::string> sortFunc[] = {
            simpleInsertSort,
            shellSort,
            heapSort,
            OwnHeapSort,
            QuickSort,
            RandomizedQuickSort,
            BubbleSort,
            selectSort,
            MergeSort,
    };

    std::vector<std::string> name = {
            "simpleInsertSort",
            "shellSort",
            "heapSort",
            "OwnHeapSort",
            "QuickSort",
            "RandomizedQuickSort",
            "BubbleSort",
            "selectSort",
            "MergeSort",
    };

    for (int i = 0; i < 9; i++) {
        if (test_sort(sortFunc[i]) == true) {
            std::cout << name[i] << " pass successfully!" << std::endl;
        } else {
            std::cout << name[i] << " fail!" << std::endl;
        }
    }


    return 0;
}