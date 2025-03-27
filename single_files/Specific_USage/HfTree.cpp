#include <string>
#include <iostream>
#include <iomanip> 

template <class T>
class hfTree {
private:
    struct Node {
        T data;
        int weight;
        int parent, left, right;
    };

    Node* elem;
    int length; // Fixed size of the tree

public:
    struct hfcode {
        T data;
        std::string code;
    };

    hfTree(const T* x, const int* w, int size) {
        const int MAXINT = 32767;
        int min_1, min_2; // The lowest and second lowest weights
        int least, second_least;

        // Initialize
        length = 2 * size;
        elem = new Node[length];

        // Initial values for leaf nodes
        for (int i = size; i < length; ++i) {
            elem[i].weight = w[i - size];
            elem[i].data = x[i - size];
            elem[i].parent = elem[i].right = elem[i].left = 0;
        }

        // Merge nodes to build the Huffman tree
        for (int i = size - 1; i > 0; --i) {
            min_1 = min_2 = MAXINT;
            least = second_least = 0;

            // Find the two smallest nodes without parents
            for (int j = i + 1; j < length; j++) {
                if (elem[j].parent == 0) {
                    if (elem[j].weight < min_1) {
                        min_2 = min_1;
                        min_1 = elem[j].weight;
                        second_least = least;
                        least = j;
                    } else if (elem[j].weight < min_2) {
                        min_2 = elem[j].weight;
                        second_least = j;
                    }
                }
            }

            // Update the new internal node
            elem[i].weight = min_1 + min_2;
            elem[i].left = second_least;
            elem[i].right = least;
            elem[i].parent = 0;
            elem[least].parent = i;
            elem[second_least].parent = i;
        }
    }

    // Generate Huffman codes
    void getcode(hfcode result[]) {
        int size = length / 2;
        int cur, cur_parent;

        for (int i = size; i < length; ++i) {
            result[i - size].data = elem[i].data;
            result[i - size].code = "";
            cur_parent = elem[i].parent;
            cur = i;

            while (cur_parent != 0) {
                if (elem[cur_parent].left == cur) {
                    result[i - size].code = "0" + result[i - size].code;
                } else {
                    result[i - size].code = "1" + result[i - size].code;
                }
                cur = cur_parent;
                cur_parent = elem[cur_parent].parent;
            }
        }
    }

    // Visualize the tree structure
    void viewTree() {
        std::cout << "Huffman Tree Structure:\n";
        viewTreeHelper(1, 0); // Start from root (index 1) with depth 0
    }

private:
    // Recursive helper function to print the tree
    void viewTreeHelper(int index, int depth) {
        if (index >= length || elem[index].weight == 0) return;

        // Print indentation based on depth
        std::cout << std::setw(depth * 4) << "";

        // Print node information
        if (index >= length / 2) {
            // Leaf node
            std::cout << "Leaf: " << elem[index].data << " (Weight: " << elem[index].weight << ")\n";
        } else {
            // Internal node
            std::cout << "Node (Weight: " << elem[index].weight << ")\n";
        }

        // Recursively print left and right children
        viewTreeHelper(elem[index].left, depth + 1);
        viewTreeHelper(elem[index].right, depth + 1);
    }

public:
    ~hfTree() {
        delete[] elem;
    }
};

int main() {
    // Test data
    char ch[] = {'H', 'e', 'l', 'd', 'o', 'w', 'm', 'r', 'f', 'k'};
    int w[] = {1, 4, 2, 66, 7, 8, 9, 5, 3, 6};

    hfTree<char> tree(ch, w, 10);
    hfTree<char>::hfcode result[10];

    // Generate and print Huffman codes
    tree.getcode(result);
    std::cout << "Huffman Codes:\n";
    for (int i = 0; i < 10; i++) {
        std::cout << result[i].data << ": " << result[i].code << std::endl;
    }

    // Visualize the Huffman tree
    tree.viewTree();
}
