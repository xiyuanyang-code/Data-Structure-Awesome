// Another implementation of the LCA problem using Euler Tour and Sparse Table
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

class EulerTourLCA {
private:
    vector<vector<int>> adj;// Adjacency list representation of the tree
    vector<int> euler;      // Euler tour order
    vector<int> depth;      // Depth of each node
    vector<int> firstOccur; // First occurrence of each node in the Euler tour
    vector<vector<int>> st; // Sparse table (stores indices)
    int n;                  // Number of nodes

    // Recursive function to generate Euler tour, depth, and first occurrence
    void dfs(int u, int parent, int currentDepth) {
        firstOccur[u] = euler.size();// Record the first occurrence of the node
        euler.push_back(u);          // Add the current node to the Euler tour
        depth[u] = currentDepth;     // Record the depth of the node

        for (int v : adj[u]) {
            if (v != parent) {
                dfs(v, u, currentDepth + 1);
                euler.push_back(u);// Add the current node again when backtracking
            }
        }
    }

    // Build the sparse table (preprocess for range minimum query)
    void buildSparseTable() {
        int m = euler.size();
        int logM = log2(m) + 1;
        st.resize(m, vector<int>(logM));

        // Initialize the first column (interval length = 1)
        for (int i = 0; i < m; ++i) {
            st[i][0] = i;
        }

        // Fill the sparse table using dynamic programming
        for (int j = 1; (1 << j) <= m; ++j) {
            for (int i = 0; i + (1 << j) - 1 < m; ++i) {
                int mid = i + (1 << (j - 1));
                if (depth[euler[st[i][j - 1]]] < depth[euler[st[mid][j - 1]]]) {
                    st[i][j] = st[i][j - 1];
                } else {
                    st[i][j] = st[mid][j - 1];
                }
            }
        }
    }

    // Query the index of the node with the minimum depth in the range [l, r]
    int queryMinIndex(int l, int r) {
        if (l > r) swap(l, r);
        int k = log2(r - l + 1);
        int mid = r - (1 << k) + 1;

        if (depth[euler[st[l][k]]] < depth[euler[st[mid][k]]]) {
            return st[l][k];
        } else {
            return st[mid][k];
        }
    }

public:
    EulerTourLCA(int numNodes) : n(numNodes), adj(numNodes), firstOccur(numNodes), depth(numNodes) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void preprocess() {
        dfs(0, -1, 0);// Assume the root node is 0
        buildSparseTable();
    }

    int findLCA(int u, int v) {
        int l = firstOccur[u];
        int r = firstOccur[v];
        int index = queryMinIndex(l, r);
        return euler[index];
    }
};

int main() {
    int n = 10;
    EulerTourLCA solver(n);

    // Build the tree (same structure as the original problem)
    solver.addEdge(0, 1);
    solver.addEdge(0, 2);
    solver.addEdge(1, 3);
    solver.addEdge(1, 4);
    solver.addEdge(2, 5);
    solver.addEdge(2, 6);
    solver.addEdge(5, 7);
    solver.addEdge(5, 8);
    solver.addEdge(7, 9);

    solver.preprocess();// Preprocess the tree

    // Validate test cases
    cout << "LCA(3,4) = " << solver.findLCA(3, 4) << endl;// Should output 1
    cout << "LCA(3,9) = " << solver.findLCA(3, 9) << endl;// Should output 0
    cout << "LCA(8,9) = " << solver.findLCA(8, 9) << endl;// Should output 5
    cout << "LCA(7,0) = " << solver.findLCA(7, 0) << endl;// Should output 0

    return 0;
}