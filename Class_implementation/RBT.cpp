/*
 * @Author: Xiyuan Yang   xiyuan_yang@outlook.com
 * @Date: 2025-04-10 15:01:57
 * @LastEditors: Xiyuan Yang   xiyuan_yang@outlook.com
 * @LastEditTime: 2025-04-10 20:10:18
 * @FilePath: /Data_structure/Class_implementation/RBT.cpp
 * @Description: 
 * Do you code and make progress today?
 * Copyright (c) 2025 by Xiyuan Yang, All Rights Reserved. 
 */
#include <cstdio>

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
class RBT {
    enum colourT { RED,
                   BLACK };
    struct RBTNode {
        enum colourT colour;
        set<Key, Other> data;
        RBTNode *left;
        RBTNode *right;

        // constrcutor
        RBTNode(const set<Key, Other> &element, RBTNode *left_ = nullptr, RBTNode *right_ = nullptr, colourT colour_ = RED)
            : data(element), left(left_), right(right_), colour(colour_) {}
    };

private:
    RBTNode *root;

private:
    // several private functions
    void makeEmpty(RBTNode *&t) {
        if (t != nullptr) {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
        }
        t = nullptr;
    }

    void insertAdjust(RBTNode *grandparent, RBTNode *parent, RBTNode *t) {
        if (parent->colour == BLACK) {
            // the parent node is BLACK, which means there is no collusion!
            return;
        }

        // for all the situations below, parent is red
        if (parent == root) {
            // make sure the root node is black
            parent->colour = BLACK;
            return;
        }

        // for the 4 rotations
        if (grandparent->left == parent) {
            if (parent->left == t) {
                LL(grandparent);
            } else {
                LR(grandparent);
            }
        } else {
            if (parent->right == t) {
                RR(grandparent);
            } else {
                RL(grandparent);
            }
        }
    }

    void removeAdjust(RBTNode *grandparent, RBTNode *parent, RBTNode *t) {}

    void LL(RBTNode *grandparent) {
        RBTNode *p = grandparent->left, *t = p->left;

        // store the RBT tree, we just copy all the tree!(shallow copy,but for data only)
        RBTNode tmp = *grandparent;

        grandparent->data = p->data;
        grandparent->left = t;
        grandparent->right = p;

        p->data = tmp.data;
        p->left = p->right;
        p->right = tmp.right;
    }

    void LR(RBTNode *grandparent) {
        RBTNode *p = grandparent->left, *t = p->right;

        // store the RBT tree, we just copy all the tree!(shallow copy,but for data only)
        RBTNode tmp = *grandparent;

        grandparent->data = t->data;
        grandparent->right = t;
        p->right = t->left;

        t->data = tmp.data;
        t->left = t->right;
        t->right = tmp.right;
    }

    void RR(RBTNode *grandparent) {
        RBTNode *p = grandparent->right, *t = p->right;

        // store the RBT tree, we just copy all the tree!(shallow copy,but for data only)
        RBTNode tmp = *grandparent;

        grandparent->data = p->data;
        grandparent->right = t;
        grandparent->left = p;

        p->data = tmp.data;
        p->right = p->left;
        p->left = tmp.left;
    }

    void RL(RBTNode *grandparent) {
        RBTNode *p = grandparent->right, *t = p->left;

        // store the RBT tree, we just copy all the tree!(shallow copy,but for data only)
        RBTNode tmp = *grandparent;

        grandparent->data = t->data;
        grandparent->left = t;
        p->left = t->right;

        t->data = tmp.data;
        t->right = t->left;
        t->left = tmp.left;
    }

public:
    /**
     * @brief Construct a new RBT object
     * 
     * @param t the current root node
     */
    RBT(RBTNode *t = nullptr) {
        root = t;
    }

    /**
     * @brief Destroy the RBT object (Using makeempty)
     * 
     */
    ~RBT() {
        makeEmpty(root);
    }

    set<Key, Other> *find(const Key &x) const {
        RBTNode *t = root;
        // iterate from the root
        while (t != nullptr && t->data != x) {
            if (t->data.key > x) {
                t = t->left;
            } else {
                t = t->right;
            }
        }

        if (t == nullptr) {
            // find nothing
            return nullptr;
        } else {
            return (set<Key, Other> *) t;
        }
    }

    /**
     * @brief insert the element x in the RBT Tree
     * 
     * @param x 
     */
    void insert(const set<Key, Other> &x) {
        RBTNode *grandparent, *parent, *t;

        if (root == nullptr) {
            // insert in the empty tree
            root = new RBTNode(x, nullptr, nullptr, BLACK);
            return;
        }

        grandparent = parent = t = root;
        // from the top to the bottom, adjust the structure
        while (true) {
            if (t != nullptr) {
                // the two child are red while t is black
                if (t->left && t->left->colour == RED && t->right && t->right->colour == RED) {
                    // swap the colour and then adjust the tree
                    t->left->colour = t->right->colour = BLACK;
                    t->colour = RED;
                    insertAdjust(grandparent, parent, t);
                }
                // for other cases, we don't do anything
                // update the three nodes
                grandparent = parent;
                parent = t;
                t = (t->data.key > x.key) ? t->left : t->right;
            } else {
                // reach the bottom, the nullptr
                t = new RBTNode(x);

                // place the new node to the "right" place
                if (x.key < parent->data.key) {
                    parent->left = t;
                } else {
                    parent->right = t;
                }

                insertAdjust(grandparent, parent, t);

                // make sure after the adjustment, the root node remain black
                root->colour = BLACK;
                return;
            }
        }
    }

    void remove(const Key &x) {
    }
};