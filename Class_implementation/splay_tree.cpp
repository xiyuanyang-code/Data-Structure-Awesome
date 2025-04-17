#include <iostream>

template<typename Key, typename Value>
class SplayTree {
private:
    struct Node {
        Key key;
        Value value;
        Node *left;
        Node *right;

        Node(Key k, Value v) : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    Node *root;

    // Right rotation
    Node *rotateRight(Node *node) {
        Node *newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        return newRoot;
    }

    // Left rotation
    Node *rotateLeft(Node *node) {
        Node *newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        return newRoot;
    }

    /**
     * @brief splay the node to the root node with the value key
     * 
     * @param node the original root node
     * @param key the value to be splayed to the root node
     * @return Node* 
     */
    Node *splay(Node *node, Key key) {
        if (!node || node->key == key) {
            return node;
        }

        // Key lies in the left subtree
        if (key < node->key) {
            if (!node->left) return node;

            // Zig-Zig (Left Left)
            if (key < node->left->key) {
                node->left->left = splay(node->left->left, key);
                node = rotateRight(node);
            }
            // Zig-Zag (Left Right)
            else if (key > node->left->key) {
                node->left->right = splay(node->left->right, key);
                if (node->left->right) {
                    node->left = rotateLeft(node->left);
                }
            }

            return node->left ? rotateRight(node) : node;
        }
        // Key lies in the right subtree
        else {
            if (!node->right) return node;

            // Zag-Zig (Right Left)
            if (key < node->right->key) {
                node->right->left = splay(node->right->left, key);
                if (node->right->left) {
                    node->right = rotateRight(node->right);
                }
            }
            // Zag-Zag (Right Right)
            else if (key > node->right->key) {
                node->right->right = splay(node->right->right, key);
                node = rotateLeft(node);
            }

            return node->right ? rotateLeft(node) : node;
        }
    }

    // Helper function to delete a node
    Node *deleteNode(Node *node, Key key) {
        if (!node) return nullptr;

        node = splay(node, key);

        // If the key is not found, return the original tree
        if (node->key != key) return node;

        // If the key is found, delete the root node
        if (!node->left) {
            Node *temp = node->right;
            delete node;
            return temp;
        } else {
            Node *temp = node->left;
            temp = splay(temp, key);
            temp->right = node->right;
            delete node;
            return temp;
        }
    }

public:
    SplayTree() : root(nullptr) {}

    // Insert a key-value pair
    void insert(Key key, Value value) {
        if (!root) {
            root = new Node(key, value);
            return;
        }

        root = splay(root, key);

        if (root->key == key) {
            root->value = value;// Update value if key already exists
            return;
        }

        Node *newNode = new Node(key, value);
        if (key < root->key) {
            newNode->right = root;
            newNode->left = root->left;
            root->left = nullptr;
        } else {
            newNode->left = root;
            newNode->right = root->right;
            root->right = nullptr;
        }
        root = newNode;
    }

    // Find a key and return its value
    Value *find(Key key) {
        root = splay(root, key);
        if (root && root->key == key) {
            return &root->value;
        }
        return nullptr;
    }

    // Delete a key
    void remove(Key key) {
        root = deleteNode(root, key);
    }

    // Print the tree (in-order traversal)
    void print() {
        if (root == nullptr) {
            std::cout << "The Tree is empty" << std::endl;
            return;
        }
        printHelper(root);
        std::cout << std::endl;
    }

private:
    void printHelper(Node *node) {
        if (!node) return;
        printHelper(node->left);
        std::cout << "(" << node->key << ", " << node->value << ") ";
        printHelper(node->right);
    }
};

int main() {
    SplayTree<int, std::string> tree;

    // Insert elements
    std::cout << "Inserting elements into the tree:" << std::endl;
    tree.insert(10, "Ten");
    tree.insert(20, "Twenty");
    tree.insert(30, "Thirty");
    tree.insert(5, "Five");
    tree.insert(15, "Fifteen");
    tree.insert(25, "Twenty-Five");
    tree.insert(35, "Thirty-Five");
    tree.print();

    // Find elements
    std::cout << "\nFinding key 20:" << std::endl;
    auto value = tree.find(20);
    if (value) {
        std::cout << "Found: " << *value << std::endl;
    } else {
        std::cout << "Key not found." << std::endl;
    }

    std::cout << "\nFinding key 40 (non-existent):" << std::endl;
    value = tree.find(40);
    if (value) {
        std::cout << "Found: " << *value << std::endl;
    } else {
        std::cout << "Key not found." << std::endl;
    }

    // Delete elements
    std::cout << "\nDeleting key 10:" << std::endl;
    tree.remove(10);
    tree.print();

    std::cout << "\nDeleting key 30:" << std::endl;
    tree.remove(30);
    tree.print();

    std::cout << "\nDeleting key 5:" << std::endl;
    tree.remove(5);
    tree.print();

    std::cout << "\nDeleting key 40 (non-existent):" << std::endl;
    tree.remove(40);
    tree.print();

    // Edge case: Insert duplicate key
    std::cout << "\nInserting duplicate key 20 with new value 'Updated Twenty':" << std::endl;
    tree.insert(20, "Updated Twenty");
    tree.print();

    // Edge case: Insert into an empty tree
    std::cout << "\nClearing the tree and inserting into an empty tree:" << std::endl;
    tree.remove(15);
    tree.remove(25);
    tree.remove(35);
    tree.remove(20);
    tree.print();

    tree.insert(50, "Fifty");
    tree.print();

    return 0;
}