/*
 * @Author: Xiyuan Yang   xiyuan_yang@outlook.com
 * @Date: 2025-04-10 15:01:57
 * @LastEditors: Xiyuan Yang   xiyuan_yang@outlook.com
 * @LastEditTime: 2025-04-10 21:54:00
 * @FilePath: /Data_structure/Class_implementation/RBT.cpp
 * @Description: 
 * Do you code and make progress today?
 * Copyright (c) 2025 by Xiyuan Yang, All Rights Reserved. 
 */
#include <cstdio>
#include <iostream>

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

    /**
     * @brief adjust the structure after the insertion and modifications
     * 
     * @param grandparent 
     * @param parent 
     * @param t the current node X
     */
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

    /**
     * @brief adjust the tree structure when deletion happens
     * 
     * @param parent 
     * @param current 
     * @param brother brother node of the current node
     */
    void removeAdjust(RBTNode *parent, RBTNode *current, RBTNode *brother, Key del) {
        if (current->colour == RED) {
            // no adjustment happens, all is well
            return;
        }

        if (current == root) {
            if (current->left != nullptr && current->right != nullptr && current->right->colour == current->left->colour) {
                current->colour = RED;
                current->left->colour = current->right->colour = BLACK;
                return;
            }
        }

        if ((current->left != nullptr && current->left->colour == BLACK || current->left == nullptr) &&
            (current->right != nullptr && current->right->colour == BLACK || current->right == nullptr)) {
            // has both black child or nullptr
            if ((brother->left != nullptr && brother->left->colour == BLACK || brother->left == nullptr) &&
                (brother->right != nullptr && brother->right->colour == BLACK || brother->right == nullptr)) {
                parent->colour = BLACK;
                brother->colour = current->colour = RED;
            } else {
                if (parent->left == brother) {
                    if (brother->left != nullptr && brother->left->colour == RED) {
                        brother->left->colour = BLACK;
                        LL(parent);
                        parent = brother;
                    } else {
                        LR(parent);
                        parent = parent->right;
                        parent->colour = BLACK;
                    }
                } else if (brother->right && brother->right->colour == RED) {
                    brother->right->colour = BLACK;
                    RR(parent);
                    parent = brother;
                } else {
                    RL(parent);
                    parent = parent->left;
                    parent->colour = BLACK;
                }
                current->colour = RED;
            }
        } else {
            // has at least one red child
            if (current->data.key == del) {
                // current node is the node tobe deleted
                if (current->left != nullptr && current->right != nullptr) {
                    if (current->right->colour == BLACK) {
                        LL(current);
                        current = current->right;
                    }
                    return;
                }

                if (current->left != nullptr) {
                    LL(current);
                    parent = current;
                    current = current->right;
                } else {
                    RR(current);
                    parent = current;
                    current = current->right;
                }
            } else {
                // the node is not the node to be deleted
                // update the node
                parent = current;
                current = (del < parent->data.key) ? parent->left : parent->right;
                brother = (current == parent->left) ? parent->right : parent->left;

                if (current->colour == BLACK) {
                    if (brother == parent->right) {
                        RR(parent);
                    } else {
                        LL(parent);
                    }

                    parent = brother;
                    brother = (current == parent->left) ? parent->right : parent->left;
                    removeAdjust(parent, current, brother, del);
                }
            }
        }
    }

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
        while (t != nullptr && t->data.key != x) {
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
        Key del = x;
        RBTNode *current, *parent, *brother;

        if (root == nullptr) {
            return;
        }

        // the tree has only one element
        if (root->data.key == x && root->left == nullptr && root->right == nullptr) {
            delete root;
            root = nullptr;
            return;
        }

        parent = current = brother = root;
        while (true) {
            removeAdjust(parent, current, brother, del);
            if (current->data.key == del && current->left != nullptr && current->right != nullptr) {
                // has two child and is just the node to be deleted
                RBTNode *tmp = current->right;
                while (tmp->left != nullptr) {
                    tmp = tmp->left;
                }
                // find the first element in the right-subtree
                current->data = tmp->data;
                del = tmp->data.key;
                parent = current;
                current = current->right;
                brother = parent->left;
            }

            if (current->data.key == del) {
                // has at least most child
                // delete the current node
                delete current;
                if (parent->left == current) {
                    parent->left = nullptr;
                } else {
                    parent->right = nullptr;
                }

                // upwards the node of current
                current = (del < parent->data.key) ? parent->left : parent->right;
            }

            // update the nodes
            parent = current;
            current = (del < parent->data.key) ? parent->left : parent->right;
        }
    }
};


int main() {
    // Create an RBT instance
    RBT<int, std::string> rbt;

    // Insert elements into the RBT
    std::cout << "Inserting elements into the RBT..." << std::endl;
    rbt.insert({10, "Ten"});
    rbt.insert({20, "Twenty"});
    rbt.insert({30, "Thirty"});
    rbt.insert({15, "Fifteen"});
    rbt.insert({25, "Twenty-Five"});
    rbt.insert({5, "Five"});

    // Find elements in the RBT
    std::cout << "\nFinding elements in the RBT..." << std::endl;
    auto result = rbt.find(15);
    if (result != nullptr) {
        std::cout << "Found: " << result->key << " -> " << result->other << std::endl;
    } else {
        std::cout << "Key 15 not found in the RBT." << std::endl;
    }

    result = rbt.find(100);
    if (result != nullptr) {
        std::cout << "Found: " << result->key << " -> " << result->other << std::endl;
    } else {
        std::cout << "Key 100 not found in the RBT." << std::endl;
    }

    // Remove elements from the RBT
    std::cout << "\nRemoving elements from the RBT..." << std::endl;
    rbt.remove(15);
    std::cout << "Removed key 15." << std::endl;

    result = rbt.find(15);
    if (result != nullptr) {
        std::cout << "Found: " << result->key << " -> " << result->other << std::endl;
    } else {
        std::cout << "Key 15 not found in the RBT after removal." << std::endl;
    }

    rbt.remove(10);
    std::cout << "Removed key 10." << std::endl;

    result = rbt.find(10);
    if (result != nullptr) {
        std::cout << "Found: " << result->key << " -> " << result->other << std::endl;
    } else {
        std::cout << "Key 10 not found in the RBT after removal." << std::endl;
    }

    std::cout << "\nRBT test completed." << std::endl;

    return 0;
}