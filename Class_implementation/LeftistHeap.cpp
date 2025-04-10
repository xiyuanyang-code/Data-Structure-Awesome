/*
 * @Author: Xiyuan Yang   xiyuan_yang@outlook.com
 * @Date: 2025-03-27 17:25:03
 * @LastEditors: Xiyuan Yang   xiyuan_yang@outlook.com
 * @LastEditTime: 2025-04-10 13:18:05
 * @FilePath: /Data_structure/Class_implementation/LeftistHeap.cpp
 * @Description: 
 * Do you code and make progress today?
 * Copyright (c) 2025 by Xiyuan Yang, All Rights Reserved. 
 */
#include <iostream>

struct LeftistNode {
    int key;
    int npl;// Null Path Length
    LeftistNode *left;
    LeftistNode *right;

    LeftistNode(int k) : key(k), npl(0), left(nullptr), right(nullptr) {}
};

class LeftistHeap {
public:
    LeftistHeap() : root(nullptr) {}

    void insert(int key) {
        LeftistNode *newNode = new LeftistNode(key);
        root = merge(root, newNode);
    }

    void deleteMin() {
        if (!root) return;
        LeftistNode *temp = root;
        root = merge(root->left, root->right);
        delete temp;
    }

    int getMin() const {
        if (!root) throw std::runtime_error("Heap is empty!");
        return root->key;
    }

private:
    LeftistNode *root;

    LeftistNode *merge(LeftistNode *h1, LeftistNode *h2) {
        if (!h1) return h2;
        if (!h2) return h1;

        if (h1->key > h2->key) std::swap(h1, h2);

        h1->right = merge(h1->right, h2);

        if (!h1->left || h1->left->npl < h1->right->npl) {
            std::swap(h1->left, h1->right);
        }

        h1->npl = h1->right ? h1->right->npl + 1 : 0;
        return h1;
    }
};

int main() {
    const int size = 10;
    int unsorted[size] = {1, 4, 6, 7, 2, 3, 5, 8, 92, 31};

    //creating a new leftist heap
    LeftistHeap lfh;
    for (int i = 0; i < size; ++i) {
        lfh.insert(unsorted[i]);
    }

    int *sorted = new int[size];
    for (int i = 0; i < size; ++i) {
        sorted[i] = lfh.getMin();
        lfh.deleteMin();
    }

    for (int i = 0; i < size; i++) {
        std::cout << sorted[i] << std::endl;
    }

    delete[] sorted;
}