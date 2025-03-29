#include <iostream>

template <class T>
class priorityQueue {
private:
    size_t currentsize;
    T* array;
    size_t maxsize;

    void doublespace() {
        T* tmp = array;
        maxsize *= 2;
        array = new T[maxsize];
        for (size_t i = 0; i <= currentsize; ++i) {
            array[i] = tmp[i];
        }
        delete[] tmp;
    }

    void buildheap() {
        for (size_t i = currentsize / 2; i >= 1; --i) {
            percolateDown(i);
        }
    }

    void percolateDown(size_t hole) {
        size_t child = 0;
        T tmp = array[hole];

        while (hole * 2 <= currentsize) {
            child = hole * 2;
            if (child < currentsize && array[child + 1] < array[child]) {
                ++child;
            }

            if (array[child] < tmp) {
                array[hole] = array[child];
                hole = child;
            } else {
                break;
            }
        }
        array[hole] = tmp;
    }

public:
    priorityQueue(size_t capacity = 100) : currentsize(0), maxsize(capacity) {
        array = new T[maxsize];
    }

    ~priorityQueue() {
        delete[] array;
    }

    priorityQueue(const T* data, size_t size) : currentsize(size), maxsize(size + 10) {
        array = new T[maxsize];
        for (size_t i = 0; i < size; ++i) {
            array[i + 1] = data[i];
        }
        buildheap();
    }

    bool empty() const {
        return currentsize == 0;
    }

    void enQueue(const T& x) {
        if (currentsize == maxsize - 1) {
            doublespace();
        }

        size_t hole = ++currentsize;
        while (hole > 1 && x < array[hole / 2]) {
            array[hole] = array[hole / 2];
            hole /= 2;
        }
        array[hole] = x;
    }

    T deQueue() {
        if (empty()) {
            throw std::underflow_error("Priority queue is empty");
        }

        T minItem = array[1];
        array[1] = array[currentsize--];
        percolateDown(1);
        return minItem;
    }

    T getHead() const {
        if (empty()) {
            throw std::underflow_error("Priority queue is empty");
        }
        return array[1];
    }
};

template <typename T>
void pqsort(T* unsorted, T* sorted, size_t size) {
    priorityQueue<T> pq(unsorted, size);
    for (size_t i = 0; i < size; ++i) {
        sorted[i] = pq.deQueue();
    }
}

int main() {
    int size = 5;
    int unsorted[] = {1, 4, 2, 6, 3};
    int* sorted = new int[size];

    try {
        pqsort(unsorted, sorted, size);

        for (size_t i = 0; i < size; ++i) {
            std::cout << sorted[i] << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    delete[] sorted;
    return 0;
}