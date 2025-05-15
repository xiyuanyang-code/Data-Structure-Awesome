/*
implementation of BFS search
*/

#include <iostream>
#include <queue>
#include <vector>

class Tree {
private:
    std::vector<std::vector<int>> adjList;// Adjacency list representation of the tree

public:
    // Constructor to initialize the tree with n nodes
    Tree(int n) {
        adjList.resize(n + 1);// Nodes are 1-indexed
    }

    // Add an edge to the tree
    void addEdge(int u, int v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u);// Since it's an undirected tree
    }

    // Perform BFS and return the BFS order
    std::vector<int> getBFSOrder(int root) {
        std::vector<int> bfsOrder;
        std::vector<bool> visited(adjList.size(), false);
        std::queue<int> q;

        visited[root] = true;
        q.push(root);

        while (!q.empty()) {
            int node = q.front();
            q.pop();
            bfsOrder.push_back(node);

            for (int child : adjList[node]) {
                if (!visited[child]) {
                    visited[child] = true;
                    q.push(child);
                }
            }
        }
        return bfsOrder;
    }
};

int main() {
    // Create a tree with 10 nodes
    Tree tree(10);

    // Add edges to the tree
    tree.addEdge(1, 2);
    tree.addEdge(1, 3);
    tree.addEdge(2, 4);
    tree.addEdge(2, 5);
    tree.addEdge(3, 6);
    tree.addEdge(3, 7);
    tree.addEdge(4, 8);
    tree.addEdge(6, 9);
    tree.addEdge(6, 10);

    // Get the BFS order starting from node 1
    std::cout << "BFS Order starting from node 1:" << std::endl;
    std::vector<int> bfsOrder = tree.getBFSOrder(1);

    // Print the BFS order
    for (int node : bfsOrder) {
        std::cout << node << " ";
    }
    std::cout << std::endl;

    return 0;
}