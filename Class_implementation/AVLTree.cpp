/*
The implementation of AVL Tree, letting the tree maintain balanced
*/

#include <cmath>
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


template<class KEY, class OTHER>
class AVLTree : public dynamicSearchTable<KEY, OTHER> {
    struct AVLNode {
        set<KEY, OTHER> data;
        AVLNode *left;
        AVLNode *right;

        // The height of the node,letting the Tree remain balanced
        int height;

        AVLNode(const set<KEY, OTHER> &element, AVLNode *left_, AVLNode *right_, int h = 1)
            : data(element), left(left_), right(right_), height(h) {}
    };

private:
    AVLNode *root;

private:
    // Tool functions

    /**
         * @brief Update the height of the node
         * 
         * @param root 
         */
    void updateHeight(AVLNode *&root_) {
        if (root_ == nullptr) {
            return;
        } else {
            root_->height = std::max(height(root_->left), height(root_->right)) + 1;
        }
    }

    /**
         * @brief insert the new element in the tree, adjusting structures when necessary
         * 
         * @param x 
         * @param root_ 
         */
    void insert(const set<KEY, OTHER> &x, AVLNode *&root_) {
        if (root_ == nullptr) {
            // insert in an empty tree
            root_ = new AVLNode(x, nullptr, nullptr);
        } else if (x.key < root_->data.key) {
            // insert in the left tree
            insert(x, root_->left);

            // judging whether is is out of balanced
            if (height(root_->left) - height(root_->right) == 2) {
                // LL rotations or LR rotations
                if (x.key < root_->left->data.key) {
                    LL(root_);
                } else {
                    LR(root_);
                }
            }
        } else if (x.key > root_->data.key) {
            // insert in the right tree
            insert(x, root_->right);

            // judging whether it is out of balanced
            if (height(root_->right) - height(root_->left) == 2) {
                // RR rotations or RL rotations
                if (x.key > (root_->right->data).key) {
                    RR(root_);
                } else {
                    RL(root_);
                }
            }
        }
        // Repetetion makes no operations

        // Update the height properties
        updateHeight(root_);
    }

    /**
         * @brief Remove the element with the value x
         * 
         * @param x key value
         * @param root_ current root node
         * @return true if the tree is balanced after the deletion
         * @return false 
         */
    bool remove(const KEY &x, AVLNode *&root_) {
        if (root_ == nullptr) {
            return true;
        }

        if (x == (root_->data).key) {
            // delete the root node
            if (root_->left == nullptr || root_->right == nullptr) {
                // The delete node has at most one child (just swap the position)
                AVLNode *oldNode = root_;

                if (root_->left == nullptr) {
                    root_ = root_->right;
                } else {
                    root_ = root_->left;
                }

                delete oldNode;
                return false;
            } else {
                // The deleted Node has two child node
                AVLNode *tmp = root_->right;
                // Go to the lead node
                while (tmp->left != nullptr) {
                    tmp = tmp->left;
                }

                root_->data = tmp->data;
                // then remove the leaf node (tmp)
                if (remove(tmp->data.key, root_->right)) {
                    return true;
                }

                return adjust(root_, 1);
            }
        }

        if (x < (root_->data).key) {
            // delete on the left subtree
            if (remove(x, root_->left)) {
                return true;
            } else {
                return adjust(root_, 0);
            }
        } else {
            // delete on the right subtree
            if (remove(x, root_->right)) {
                return true;
            } else {
                return adjust(root_, 1);
            }
        }
    }

    /**
         * @brief Make the AVL tree empty
         * 
         * @param root_ current root node
         */
    void makeEmpty(AVLNode *root_) {
        if (root_ == nullptr) {
            return;
        }

        makeEmpty(root_->left);
        makeEmpty(root_->right);
        delete root_;
    }

    /**
         * @brief Return the height of the tree
         * 
         * @param root_ 
         * @return int 
         */
    int height(AVLNode *root_) const {
        if (root_ == nullptr) {
            return 0;
        } else {
            return root_->height;
        }
    }

    // Several Rotations
    /**
         * @brief Rotate if the insertion happens on the left child of the left child
         * 
         * @param danger danger node
         */
    void LL(AVLNode *&danger) {
        AVLNode *t1 = danger->left;

        // rotate to adjust the structure
        danger->left = t1->right;
        t1->right = danger;

        // update height
        updateHeight(danger);
        updateHeight(t1);
    }

    /**
         * @brief Rotation if the insertion happens in the right child of the right child of the danger node
         * 
         * @param danger The danger node
         */
    void RR(AVLNode *&danger) {
        // The same with LL rotations
        AVLNode *t1 = danger->right;

        // rotate RR
        danger->right = t1->left;
        t1->left = danger;

        // update height
        updateHeight(danger);
        updateHeight(t1);
    }

    /**
         * @brief danger.leftchild.rightchild = insertion_place
         * 
         * @param danger 
         */
    void LR(AVLNode *&danger) {
        RR(danger->left);
        LL(danger);
    }

    /**
         * @brief danger.rightchild.leftchild = insertion_place
         * 
         * @param root_ 
         */
    void RL(AVLNode *&danger) {
        LL(danger->right);
        RR(danger);
    }

    /**
         * @brief Judge whether the tree remain the same height after the deletion
         * 
         * @param root_ the current root node
         * @param subTree 1 for deleting on the right child, 0 for left child
         * @return true 
         * @return false 
         */
    bool adjust(AVLNode *&root_, int subTree) {
        if (subTree) {
            // Delete on the right child
            if (height(root_->left) - height(root_->right) == 1) {
                // Case1: the height remains unchanged
                return true;
            } else if (height(root_->left) == height(root_->right)) {
                --(root_->height);
                return false;
            } else if (height((root_->left)->right) > height((root_->left)->left)) {
                LR(root_);
                // The height changed!
                return false;
            } else {
                LL(root_);
                if (height(root_->right) == height(root_->left)) {
                    return false;
                } else {
                    return true;
                }
            }
        } else {
            if (height(root_->right) - height(root_->left) == 1) {
                // Case1: the height remains unchanged
                return true;
            } else if (height(root_->left) == height(root_->right)) {
                --(root_->height);
                return false;
            } else if (height((root_->right)->left) > height((root_->right)->right)) {
                RL(root_);
                // The height changed!
                return false;
            } else {
                RR(root_);
                if (height(root_->right) == height(root_->left)) {
                    return false;
                } else {
                    return true;
                }
            }
        }
    }

public:
    /**
         * @brief Construct a new AVLTree object
         * 
         */
    AVLTree() {
        root = nullptr;
    }

    // destructor
    ~AVLTree() {
        makeEmpty(root);
    }

    /**
         * @brief Finding the elements with the key value of x, the same with BST
         * 
         * @param x 
         * @return set<KEY, OTHER>* 
         */
    set<KEY, OTHER> *find(const KEY &x) const {
        AVLNode *t = root;
        while (t != nullptr && t->data.key != x) {
            if (t->data.key > x) {
                // skip to the left tree
                t = t->left;
            } else {
                t = t->right;
            }
        }

        if (t == nullptr) {
            return nullptr;
        } else {
            return (set<KEY, OTHER> *) t;
        }
    }

    void insert(const set<KEY, OTHER> &x) {
        insert(x, root);
    }

    void remove(const KEY &x) {
        remove(x, root);
    }
};


int main() {
    // Create an AVLTree instance
    AVLTree<int, std::string> tree;

    // Insert elements into the AVL tree
    std::cout << "Inserting elements into the AVL tree..." << std::endl;
    tree.insert({10, "Ten"});
    tree.insert({20, "Twenty"});
    tree.insert({30, "Thirty"});
    tree.insert({40, "Forty"});
    tree.insert({50, "Fifty"});
    tree.insert({25, "Twenty-Five"});

    // Search for an element in the AVL tree
    std::cout << "Searching for key 20 in the AVL tree..." << std::endl;
    auto result = tree.find(20);
    if (result != nullptr) {
        std::cout << "Found: " << result->key << " -> " << result->other << std::endl;
    } else {
        std::cout << "Key 20 not found in the AVL tree." << std::endl;
    }

    // Remove an element from the AVL tree
    std::cout << "Removing key 20 from the AVL tree..." << std::endl;
    tree.remove(20);

    // Search for the removed element
    std::cout << "Searching for key 20 after removal..." << std::endl;
    result = tree.find(20);
    if (result != nullptr) {
        std::cout << "Found: " << result->key << " -> " << result->other << std::endl;
    } else {
        std::cout << "Key 20 not found in the AVL tree." << std::endl;
    }

    // Insert more elements to test balancing
    std::cout << "Inserting more elements to test balancing..." << std::endl;
    tree.insert({5, "Five"});
    tree.insert({15, "Fifteen"});

    // Final search to verify tree structure
    std::cout << "Searching for key 15 in the AVL tree..." << std::endl;
    result = tree.find(15);
    if (result != nullptr) {
        std::cout << "Found: " << result->key << " -> " << result->other << std::endl;
    } else {
        std::cout << "Key 15 not found in the AVL tree." << std::endl;
    }

    std::cout << "AVL tree test completed." << std::endl;

    return 0;
}