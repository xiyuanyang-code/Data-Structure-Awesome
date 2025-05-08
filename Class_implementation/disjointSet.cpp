#include <cstddef>
#include <iostream>

class disjointSet {
private:
    size_t size;
    int *parent;

public:
    /**
     * @brief Construct a new disjoint Set object
     * 
     * @param s 
     */
    disjointSet(size_t s) : size(s) {
        parent = new int[size];

        // initiallize
        for (int i = 0; i < size; i++) {
            parent[i] = -1;
        }
    }

    /**
     * @brief Destroy the disjoint Set object
     * 
     */
    ~disjointSet() {
        delete parent;
    }

    /**
     * @brief Union two subtree, merged by scale
     * 
     * @param root1 
     * @param root2 
     */
    void Union(int root1, int root2) {
        if (root1 == root2) {
            return;
        }

        // !attention, these are all negative
        if (parent[root1] > parent[root2]) {
            parent[root2] += parent[root1];
            parent[root1] = root2;
        } else {
            parent[root1] += parent[root2];
            parent[root2] = root1;
        }
    }

    /**
     * @brief find the set which x lies in, using recursion
     * 
     * @param x 
     * @return int 
     */
    int Find(int x) {
        if (parent[x] < 0) {
            // x is the root node
            return x;
        }

        // optimize
        return (parent[x] = Find(parent[x]));
    }
};

int main() {
    // Create a disjoint set with 10 elements (0 to 9)
    disjointSet ds(10);

    // Perform some union operations
    std::cout << "Performing union operations..." << std::endl;
    ds.Union(0, 1);
    ds.Union(2, 3);
    ds.Union(4, 5);
    ds.Union(6, 7);
    ds.Union(8, 9);
    ds.Union(1, 3);// Union of sets containing 0, 1 and 2, 3
    ds.Union(5, 7);// Union of sets containing 4, 5 and 6, 7

    // Check the root of each element
    std::cout << "\nFinding roots of elements..." << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << "Element " << i << " belongs to set with root: " << ds.Find(i) << std::endl;
    }

    // Check if two elements are in the same set
    std::cout << "\nChecking if elements are in the same set..." << std::endl;
    std::cout << "Are 0 and 3 in the same set? " << (ds.Find(0) == ds.Find(3) ? "Yes" : "No") << std::endl;
    std::cout << "Are 4 and 7 in the same set? " << (ds.Find(4) == ds.Find(7) ? "Yes" : "No") << std::endl;
    std::cout << "Are 8 and 9 in the same set? " << (ds.Find(8) == ds.Find(9) ? "Yes" : "No") << std::endl;
    std::cout << "Are 0 and 9 in the same set? " << (ds.Find(0) == ds.Find(9) ? "Yes" : "No") << std::endl;

    return 0;
}
