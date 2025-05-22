#include <climits>
#include <iostream>

#define MAX_NODES 100// Maximum number of nodes in the graph

class FloydWarshall {
private:
    int distance[MAX_NODES][MAX_NODES];
    int next[MAX_NODES][MAX_NODES];
    int numNodes;

    void initialize(int graph[][MAX_NODES]) {
        for (int i = 0; i < numNodes; i++) {
            for (int j = 0; j < numNodes; j++) {
                distance[i][j] = graph[i][j];
                if (graph[i][j] != INT_MAX && i != j) {
                    next[i][j] = j;
                } else {
                    next[i][j] = -1;
                }
            }
        }
    }

public:
    FloydWarshall(int nodes) : numNodes(nodes) {
        // Initialize with maximum distances
        for (int i = 0; i < numNodes; i++) {
            for (int j = 0; j < numNodes; j++) {
                distance[i][j] = (i == j) ? 0 : INT_MAX;
                next[i][j] = -1;
            }
        }
    }

    void addEdge(int source, int destination, int weight) {
        if (source >= 0 && source < numNodes &&
            destination >= 0 && destination < numNodes) {
            distance[source][destination] = weight;
            next[source][destination] = destination;
        }
    }

    void computeShortestPaths() {
        for (int k = 0; k < numNodes; k++) {
            for (int i = 0; i < numNodes; i++) {
                for (int j = 0; j < numNodes; j++) {
                    if (distance[i][k] != INT_MAX &&
                        distance[k][j] != INT_MAX &&
                        distance[i][j] > distance[i][k] + distance[k][j]) {
                        distance[i][j] = distance[i][k] + distance[k][j];
                        next[i][j] = next[i][k];
                    }
                }
            }
        }
    }

    void printShortestPath(int source, int destination) {
        if (source < 0 || source >= numNodes ||
            destination < 0 || destination >= numNodes) {
            std::cout << "Invalid node indices." << std::endl;
            return;
        }

        if (distance[source][destination] == INT_MAX) {
            std::cout << "No path exists from node " << source
                      << " to node " << destination << "." << std::endl;
            return;
        }

        std::cout << "Shortest path from node " << source
                  << " to node " << destination << ": ";
        std::cout << source;

        int current = source;
        while (current != destination) {
            current = next[current][destination];
            std::cout << " -> " << current;
        }

        std::cout << "\nTotal distance: " << distance[source][destination]
                  << std::endl;
    }

    void printAllPairs() {
        std::cout << "All-pairs shortest paths:\n";
        for (int i = 0; i < numNodes; i++) {
            for (int j = 0; j < numNodes; j++) {
                if (distance[i][j] == INT_MAX) {
                    std::cout << "INF\t";
                } else {
                    std::cout << distance[i][j] << "\t";
                }
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    // Example graph with 4 nodes
    const int NODES = 4;
    FloydWarshall fw(NODES);

    // Add edges to the graph
    fw.addEdge(0, 1, 5);
    fw.addEdge(1, 0, 3);
    fw.addEdge(0, 3, 10);
    fw.addEdge(1, 2, 3);
    fw.addEdge(2, 3, 1);

    // Compute shortest paths
    fw.computeShortestPaths();

    // Print results
    fw.printAllPairs();
    std::cout << std::endl;

    // Print specific paths
    fw.printShortestPath(0, 3);
    fw.printShortestPath(1, 0);// No path case

    return 0;
}