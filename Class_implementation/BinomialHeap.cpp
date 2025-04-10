#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct BinomialNode {
    // Node structure for storing the tree, using linked storage
    int key;              // Key value of the node
    int degree;           // Degree of the binomial tree (number of children)
    BinomialNode *parent; // Pointer to the parent node
    BinomialNode *child;  // Pointer to the first child node
    BinomialNode *sibling;// Pointer to the next sibling node

    BinomialNode(int k) : key(k), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
};

class BinomialHeap {
public:
    BinomialHeap() : head(nullptr) {}

    // Insert a new key into the heap
    void insert(int key) {
        BinomialNode *newNode = new BinomialNode(key);// Create a new node with the given key
        BinomialHeap tempHeap;                        // Create a temporary heap
        tempHeap.head = newNode;                      // Set the new node as the head of the temporary heap
        merge(tempHeap);                              // Merge the temporary heap with the current heap
    }

    // Delete the minimum key from the heap
    void deleteMin() {
        if (!head) return;// If the heap is empty, do nothing

        BinomialNode *minNode = head;// Initialize minNode with the head of the heap
        BinomialNode *prev = nullptr;// Pointer to the previous node of minNode
        BinomialNode *curr = head;   // Pointer to traverse the heap

        // Find the minimum node in the root list
        while (curr) {
            if (curr->key < minNode->key) {
                minNode = curr;
                prev = curr;
            }
            curr = curr->sibling;
        }

        // Remove the minimum node from the root list
        if (prev) {
            prev->sibling = minNode->sibling;// Bypass the minNode
        } else {
            head = minNode->sibling;// Update the head if minNode is the first node
        }

        // Reverse the child list of minNode to create a new heap
        BinomialHeap tempHeap;
        tempHeap.head = reverseChildren(minNode->child);

        // Merge the new heap with the original heap
        merge(tempHeap);

        delete minNode;// Free the memory of the deleted node
    }

    // Get the minimum key in the heap
    int getMin() const {
        if (!head) {
            throw std::runtime_error("Heap is empty!");// Throw an exception if the heap is empty
        }
        int minVal = head->key;// Initialize minVal with the key of the head node
        for (BinomialNode *curr = head; curr; curr = curr->sibling) {
            minVal = min(minVal, curr->key);// Traverse the root list to find the minimum key
        }
        return minVal;
    }

private:
    BinomialNode *head;// Pointer to the head of the root list

    // Reverse the child list of a node
    BinomialNode *reverseChildren(BinomialNode *node) {
        BinomialNode *prev = nullptr;// Pointer to the previous node
        BinomialNode *curr = node;   // Pointer to traverse the child list
        while (curr) {
            BinomialNode *next = curr->sibling;// Store the next sibling
            curr->sibling = prev;              // Reverse the sibling pointer
            curr->parent = nullptr;            // Clear the parent pointer
            prev = curr;                       // Move prev to the current node
            curr = next;                       // Move curr to the next node
        }
        return prev;// Return the new head of the reversed list
    }

    // Merge two binomial heaps
    void merge(BinomialHeap &other) {
        BinomialNode *newHead = nullptr;// New head of the merged heap
        BinomialNode **tail = &newHead; // Pointer to the tail of the merged heap
        BinomialNode *h1 = head;        // Pointer to traverse the first heap
        BinomialNode *h2 = other.head;  // Pointer to traverse the second heap

        // Merge the root lists of the two heaps
        while (h1 && h2) {
            if (h1->degree < h2->degree) {// Compare degrees and add the smaller one to the merged list
                *tail = h1;
                h1 = h1->sibling;
            } else {
                *tail = h2;
                h2 = h2->sibling;
            }
            tail = &((*tail)->sibling);// Move the tail pointer forward
        }

        // Append the remaining nodes from either heap
        *tail = h1 ? h1 : h2;
        head = newHead;// Update the head of the merged heap

        consolidate();// Consolidate the merged heap to ensure unique degrees
    }

    // Consolidate the heap to ensure each degree appears only once
    void consolidate() {
        vector<BinomialNode *> degreeTable(32, nullptr);// Table to store nodes by degree (max degree assumed to be 32)

        BinomialNode *curr = head;// Pointer to traverse the heap
        BinomialNode *next;

        while (curr) {
            next = curr->sibling;                               // Store the next sibling
            while (degreeTable[curr->degree]) {                 // Check if there is another node with the same degree
                BinomialNode *other = degreeTable[curr->degree];// Get the other node
                if (curr->key > other->key) swap(curr, other);  // Ensure curr has the smaller key
                link(other, curr);                              // Link the two nodes
                degreeTable[curr->degree - 1] = nullptr;        // Clear the previous degree entry
            }
            degreeTable[curr->degree] = curr;// Store the current node in the table
            curr = next;                     // Move to the next node
        }

        head = nullptr;                // Reset the head of the heap
        for (auto node : degreeTable) {// Rebuild the root list from the degree table
            if (node) {
                node->sibling = head;// Add the node to the front of the root list
                head = node;
            }
        }
    }

    // Link two binomial trees together
    void link(BinomialNode *child, BinomialNode *parent) {
        child->parent = parent;        // Set the parent of the child node
        child->sibling = parent->child;// Add the child to the front of the parent's child list
        parent->child = child;         // Update the parent's child pointer
        parent->degree++;              // Increment the degree of the parent
    }
};

// Test the BinomialHeap class
int main() {
    try {
        BinomialHeap bh;

        // Insert elements into the heap
        bh.insert(10);
        bh.insert(5);
        bh.insert(7);
        bh.insert(3);
        bh.insert(8);

        // Print the minimum element
        cout << "Minimum element: " << bh.getMin() << endl;

        // Delete the minimum element and print the new minimum
        bh.deleteMin();
        cout << "Minimum element after deletion: " << bh.getMin() << endl;

        // Delete the minimum element again and print the new minimum
        bh.deleteMin();
        cout << "Minimum element after second deletion: " << bh.getMin() << endl;

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}