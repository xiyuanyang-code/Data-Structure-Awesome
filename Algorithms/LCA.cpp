#include <cstddef>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

// Disjoint Set (Union-Find) data structure
class disjointSet {
private:
    size_t size;
    int *parent;

public:
    disjointSet(size_t s) : size(s) {
        parent = new int[size];
        for (int i = 0; i < size; ++i) {
            parent[i] = -1;
        }
    }

    ~disjointSet() {
        delete[] parent;
    }

    void Union(int root1, int root2) {
        if (root1 == root2) return;

        if (parent[root1] > parent[root2]) {
            parent[root2] += parent[root1];
            parent[root1] = root2;
        } else {
            parent[root1] += parent[root2];
            parent[root2] = root1;
        }
    }

    int Find(int x) {
        if (parent[x] < 0) return x;
        return parent[x] = Find(parent[x]);
    }
};

// Tarjan's offline LCA algorithm implementation
class TarjanLCA {
private:
    vector<vector<int>> adj;
    vector<vector<pair<int, int>>> queries;
    unordered_map<int, int> lcaResult;
    vector<bool> visited;
    vector<int> ancestor; // Maps each node to its current ancestor in the tree

    void dfs(int node, int parent_node, disjointSet &ds) {
        visited[node] = true;

        // Traverse all children
        for (int neighbor : adj[node]) {
            if (neighbor == parent_node) continue;
            dfs(neighbor, node, ds);
            ds.Union(node, neighbor); // Merge child into current node
        }

        // After merging, set the ancestor of this node to itself
        ancestor[node] = node;

        // Process all queries associated with this node
        for (auto &q : queries[node]) {
            int v = q.first;
            int idx = q.second;

            if (visited[v]) {
                int root = ds.Find(v); // Get the representative of v's set
                int lca = ancestor[root]; // Use the stored ancestor
                lcaResult[idx] = lca;
            }
        }
    }

public:
    TarjanLCA(int n) : adj(n), visited(n, false), queries(n), ancestor(n, -1) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void addQuery(int u, int v, int idx) {
        queries[u].push_back({v, idx});
        queries[v].push_back({u, idx});
    }

    void computeLCA(int root, int queryCount) {
        disjointSet ds(adj.size());
        dfs(root, -1, ds);

        cout << "\nComputed LCA results:\n";
        for (int i = 0; i < queryCount; ++i) {
            cout << "LCA of query[" << i << "] is " << lcaResult[i] << endl;
        }
    }
};

int main() {
    int n = 10;
    TarjanLCA solver(n);

    // Build the tree
    solver.addEdge(0, 1);
    solver.addEdge(0, 2);
    solver.addEdge(1, 3);
    solver.addEdge(1, 4);
    solver.addEdge(2, 5);
    solver.addEdge(2, 6);
    solver.addEdge(5, 7);
    solver.addEdge(5, 8);
    solver.addEdge(7, 9);

    // Add some LCA queries
    solver.addQuery(3, 4, 0);   // Expected: 1
    solver.addQuery(3, 9, 1);   // Expected: 0
    solver.addQuery(8, 9, 2);   // Expected: 5
    solver.addQuery(7, 0, 3);   // Expected: 0

    // Run Tarjan's algorithm from root node (0)
    solver.computeLCA(0, 4);

    return 0;
}