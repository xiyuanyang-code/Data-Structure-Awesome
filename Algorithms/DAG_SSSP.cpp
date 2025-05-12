#include <climits>// For INT_MAX
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

class DAG {
private:
    unordered_map<int, vector<pair<int, int>>> adj;// Adjacency list with weights
    unordered_map<int, int> inDegree;              // In-degree count, used for topologicalSorting
    int vertex_num;

public:
    DAG(int num = 0) {
        vertex_num = num;
    }

    // Add a directed edge u -> v with weight w
    void addEdge(int u, int v, int weight) {
        adj[u].emplace_back(v, weight);
        inDegree[v]++;// Increment in-degree of v
    }

    // Perform topological sort using Kahn's algorithm
    vector<int> topologicalSort() {
        vector<int> result;
        queue<int> q;

        // Initialize queue with nodes having 0 in-degree
        for (auto node : adj) {
            int u = node.first;
            if (inDegree[u] == 0) {
                q.push(u);
            }
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            result.push_back(u);

            // Reduce in-degree of neighbors
            for (auto &neighbor : adj[u]) {
                int v = neighbor.first;
                inDegree[v]--;
                if (inDegree[v] == 0) {
                    q.push(v);
                }
            }
        }

        cout << adj.size();
        cout << result.size();
        // Check for cycles (if result size != number of nodes)
        cout << "a" << (result.size());
        if (result.size() != adj.size()) {
            cout << "Cycle detected! Not a DAG." << endl;
            return {};
        }

        return result;
    }

    // DAG Relaxation for Single Source Shortest Path (SSSP)
    vector<int> dagRelaxation(int source) {
        // Perform topological sort
        vector<int> sortedOrder = topologicalSort();
        if (sortedOrder.empty()) {
            cout << "Cannot perform DAG relaxation due to cycle." << endl;
            return {};
        }
        vector<int> dist(vertex_num + 1, INT_MAX);// Initialize distances to infinity
        dist[source] = 0;                     // Distance to source is 0


        // Relax edges in topological order
        for (int u : sortedOrder) {
            if (dist[u] == INT_MAX) {
                continue;
            }
            for (auto &neighbor : adj[u]) {
                int v = neighbor.first;
                cout << "v" << v << endl;
                int weight = neighbor.second;
                // update
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                }
            }
        }
        return dist;
    }
};

int main() {
    DAG dag(5);

    // Build a sample DAG with weights
    dag.addEdge(1, 2, 3);// Edge 1 -> 2 with weight 3
    dag.addEdge(1, 3, 6);// Edge 1 -> 3 with weight 6
    dag.addEdge(2, 4, 4);// Edge 2 -> 4 with weight 4
    dag.addEdge(3, 4, 8);// Edge 3 -> 4 with weight 8
    dag.addEdge(4, 5, 2);// Edge 4 -> 5 with weight 2

    // Perform DAG relaxation for SSSP
    int source = 1;
    vector<int> distances = dag.dagRelaxation(source);

    // Print the shortest distances from the source
    cout << "Shortest distances from node " << source << ":" << endl;
    for (int i = 1; i < distances.size(); i++) {
        if (distances[i] == INT_MAX) {
            cout << "Node " << i << ": Unreachable" << endl;
        } else {
            cout << "Node " << i << ": " << distances[i] << endl;
        }
    }

    return 0;
}