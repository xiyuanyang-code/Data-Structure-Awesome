// Using binary lifting to solve LCA problem with a tree structure
#include <cstring>
#include <iostream>

const int MAXN = 1005;
const int LOG = 20;

int tree[MAXN][MAXN]; // Tree structure using adjacency matrix
int child_count[MAXN];// Number of children for each node
int depth[MAXN];      // Depth of each node
int up[MAXN][LOG];    // up[i][j] represents the 2^j-th ancestor of node i

// Add an edge to the tree
void add_edge(int u, int v) {
    tree[u][child_count[u]++] = v;// Add v as a child of u
    tree[v][child_count[v]++] = u;// Add u as a child of v (undirected tree)
}

// DFS preprocessing to fill the `up` and `depth` arrays
void dfs(int u, int parent) {
    up[u][0] = parent;// Direct parent node
    for (int i = 1; i < LOG; ++i) {
        up[u][i] = up[up[u][i - 1]][i - 1];// Binary lifting
    }

    // Traverse all children of the current node
    for (int i = 0; i < child_count[u]; ++i) {
        int v = tree[u][i];
        if (v != parent) {
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
    }
}

// Compute the LCA of two nodes
int lca(int u, int v) {
    // Ensure u is the deeper node
    if (depth[u] < depth[v]) {
        std::swap(u, v);
    }

    // Lift u to the same depth as v
    for (int i = LOG - 1; i >= 0; --i) {
        if (depth[u] - (1 << i) >= depth[v]) {
            u = up[u][i];
        }
    }

    // If u and v are the same, return the result
    if (u == v) {
        return u;
    }

    // Lift u and v simultaneously until their parents are the same
    for (int i = LOG - 1; i >= 0; --i) {
        if (up[u][i] != up[v][i]) {
            u = up[u][i];
            v = up[v][i];
        }
    }

    // Return the LCA
    return up[u][0];
}

int main() {
    memset(tree, 0, sizeof(tree));              // Initialize the tree
    memset(child_count, 0, sizeof(child_count));// Initialize child counts

    int n = 9;

    // Manually construct the tree
    add_edge(1, 2);
    add_edge(1, 3);
    add_edge(2, 4);
    add_edge(2, 5);
    add_edge(3, 6);
    add_edge(3, 7);
    add_edge(6, 8);
    add_edge(6, 9);

    // Preprocessing
    depth[1] = 0;// Assume the root node is 1
    dfs(1, 1);   // The parent of the root node is itself

    // Manually query LCA
    std::cout << "LCA(4, 5) = " << lca(4, 5) << std::endl;// Expected: 2
    std::cout << "LCA(4, 6) = " << lca(4, 6) << std::endl;// Expected: 1
    std::cout << "LCA(3, 4) = " << lca(3, 4) << std::endl;// Expected: 1
    std::cout << "LCA(8, 9) = " << lca(8, 9) << std::endl;// Expected: 6
    std::cout << "LCA(5, 9) = " << lca(5, 9) << std::endl;// Expected: 1

    return 0;
}