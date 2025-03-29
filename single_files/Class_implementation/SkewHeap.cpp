/*
 * @Author: Xiyuan Yang   xiyuan_yang@outlook.com
 * @Date: 2025-03-27 19:10:30
 * @LastEditors: Xiyuan Yang   xiyuan_yang@outlook.com
 * @LastEditTime: 2025-03-27 19:13:34
 * @FilePath: /Data_structure/single_files/Class_implementation/skewHeap.cpp
 * @Description: 
 * Do you code and make progress today?
 * Copyright (c) 2025 by Xiyuan Yang, All Rights Reserved. 
 */

#include <iostream>
struct SkewNode {
    int key;
    SkewNode* left;
    SkewNode* right;

    SkewNode(int k) : key(k), left(nullptr), right(nullptr) {}
};

class SkewHeap {
public:
    SkewHeap() : root(nullptr) {}

    void insert(int key) {
        SkewNode* newNode = new SkewNode(key);
        root = merge(root, newNode);
    }

    void deleteMin() {
        if (!root) return;
        SkewNode* temp = root;
        root = merge(root->left, root->right);
        delete temp;
    }

    int getMin() const {
        if (!root) throw std::runtime_error("Heap is empty!");
        return root->key;
    }

private:
    SkewNode* root;

    SkewNode* merge(SkewNode* h1, SkewNode* h2) {
        if (!h1) return h2;
        if (!h2) return h1;

        if (h1->key > h2->key) std::swap(h1, h2);

        // Swap the children of h1
        h1->right = merge(h1->right, h2);
        std::swap(h1->left, h1->right);

        return h1;
    }
};


int main(){
    const int size = 10;
    int unsorted [size] = {1,4,6,7,2,3,5,8,92,31};

    //creating a new leftist heap
    SkewHeap skh;
    for(int i = 0; i < size; ++i){
        skh.insert(unsorted[i]);
    }

    int* sorted = new int [size];
    for(int i = 0; i < size; ++i){
        sorted[i] = skh.getMin();
        skh.deleteMin();
    }

    for(int i = 0; i < size; i++){
        std::cout << sorted[i] << std::endl;
    }

    delete [] sorted;

}