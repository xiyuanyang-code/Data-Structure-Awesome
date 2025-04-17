/*
 * @Author: Xiyuan Yang   xiyuan_yang@outlook.com
 * @Date: 2025-04-17 14:22:30
 * @LastEditors: Xiyuan Yang   xiyuan_yang@outlook.com
 * @LastEditTime: 2025-04-17 21:11:16
 * @FilePath: /Data_structure/Class_implementation/close_Hash_Table.cpp
 * @Description: 
 * Do you code and make progress today?
 * Copyright (c) 2025 by Xiyuan Yang, All Rights Reserved. 
 */

#include <cstddef>
#include <iostream>
#include <queue>
#include <random>
#include <stdexcept>

template<class KEY, class OTHER>
struct set {
    KEY key;
    OTHER other;
};

// Dynamic Search Table
template<class Key, class Other>
class dynamicSearchTable {
public:
    // finding elements of x
    virtual set<Key, Other> *find(const Key &x) const = 0;

    // insert element, maintaining the order
    virtual void insert(const set<Key, Other> &x) = 0;

    // remove an element, maintaining the order
    virtual void remove(const Key &x) = 0;

    virtual ~dynamicSearchTable() {}
};

template<class Key, class Other>
class close_Hash_Table : public dynamicSearchTable<Key, Other> {
private:
    struct Node {
        set<Key, Other> data;
        int status;
        // status: 0 for empty, 1 for active and 2 for deleted
        Node() {
            status = 0;
        }
    };

    Node *array;
    int size;

    // function pointer (hash function)
    int (*key)(const Key &x);

    // if the key value is int iteself, we no need for another function
    static int defaultKey(const int &x) {
        return x;
    }

public:
    close_Hash_Table(int length = 101, int (*f)(const Key &x) = defaultKey) {
        size = length;
        array = new Node[size];
        key = f;
    }
    ~close_Hash_Table() {
        delete[] array;
    }

    set<Key, Other> *find(const Key &x) const {
        int init_pos, pos;
        pos = init_pos = key(x) % size;
        do {
            if (array[pos].status == 0) {
                return nullptr;
            }

            if (array[pos].status == 1 && array[pos].data.key == x) {
                // find the element
                return (set<Key, Other> *) (array + pos);
            }

            pos = (pos + 1) % size;
        } while (pos != init_pos);

        return nullptr;
    }

    void insert(const set<Key, Other> &x) {
        int init_pos, pos;

        // get the position after the mapping
        init_pos = pos = key(x.key) % size;
        do {
            if (array[pos].status != 1) {
                // then we can insert the value!
                array[pos].data = x;
                array[pos].status = 1;
                return;
            } else {
                if (array[pos].data.key == x.key) {
                    // we have insert the same key value!
                    // return directly
                    return;
                }
            }
            pos = (pos + 1) % size;
        } while (pos != init_pos);
        // if there is no free space, then it will jump out of the while loop
        // you can throw somwething here
        throw std::runtime_error("The table has been occupied!");
    }

    void remove(const Key &x) {
        int init_pos, pos = 0;
        init_pos = pos = key(x) % size;
        do {
            if (array[pos].status == 0) {
                // no element will be deleted
                return;
            }
            if (array[pos].status == 1 && array[pos].data.key == x) {
                // find the specific element
                array[pos].status = 2;
                return;
            }
            pos = (pos + 1) % size;
        } while (pos != init_pos);
    }
};

int generate_random(int range = 100000) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, range);
    return dis(gen);
}

std::string generate_random_string(size_t length = 20) {
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, characters.size() - 1);

    std::string random_string;
    for (size_t i = 0; i < length; ++i) {
        random_string += characters[dis(gen)];
    }
    return random_string;
}


int main() {
    close_Hash_Table<int, std::string> s(10000);
    long long alldata_range = 1000000000;
    long long alldata_num = 8000;
    long long query_num = 1000000;
    set<int, std::string> element;
    std::queue<int> storage;

    for (int i = 0; i < alldata_num; i++) {
        set<int, std::string> element;
        element.key = generate_random(alldata_range);
        element.other = generate_random_string();
        s.insert(element);
        storage.push(element.key);
    }

    // for the searching period
    for (int i = 0; i < query_num; i++) {
        int search_key = generate_random(alldata_range);
        if (s.find(search_key) != nullptr) {
            std::cout << "Find the key value " << search_key << std::endl;
        }

        if (generate_random(1000) % 3 == 0 && !storage.empty()) {
            // randomly remove some elements
            int dele = storage.front();
            s.remove(dele);
            if (s.find(dele) == nullptr) {
                std::cout << "The key value " << dele << " has been deleted successfully." << std::endl;
            }
            storage.pop();
        }
    }

    return 0;
}
