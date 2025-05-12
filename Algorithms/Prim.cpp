#include <climits>
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

// implementation of Kruskal algorithms
template<class Ver, class Edge>
class AdjListGraph {
private:
    struct EdgeNode {
        int end;
        Edge weight;
        EdgeNode *next;

        EdgeNode(int e, Edge weight_, EdgeNode *next_ = nullptr) {
            end = e;
            weight = weight_;
            next = next_;
        }
    };

    struct VerNode {
        Ver value;
        EdgeNode *head;

        VerNode(EdgeNode *head_ = nullptr) {
            head = head_;
        }
    };


    int find(Ver x) const {
        for (int i = 0; i < Vers; i++) {
            if (verlist[i].value == x) {
                return i;
            }
        }
        return -1;
    }

    VerNode *verlist;
    int Vers, Edges;

public:
    AdjListGraph(int size_, const Ver *d) {
        Vers = size_;
        Edges = 0;
        verlist = new VerNode[Vers];
        for (int i = 0; i < Vers; i++) {
            verlist[i].value = d[i];
        }
    }

    ~AdjListGraph() {
        EdgeNode *p = nullptr;
        for (int i = 0; i < Vers; i++) {
            while ((p = verlist[i].head) != nullptr) {
                // delete the list
                verlist[i].head = p->next;
                delete p;
            }
        }
    }

    void insert(Ver x, Ver y, Edge w) {
        int u = find(x);
        int v = find(y);
        verlist[u].head = new EdgeNode(v, w, verlist[u].head);
        verlist[v].head = new EdgeNode(u, w, verlist[v].head);
        ++Edges;
    }

    // other functions of a basic graph skipped

    void Prim(Edge NoEdge) const {
        // NoEdge is for \infty
        bool *flag = new bool[Vers];
        Edge *lowCost = new Edge[Vers];
        int *startNode = new int[Vers];

        // lowcost i means the lowest weight for all the ver in U to the ver i in V - U
        // startNode i will explain which node in U is which satisfies the lowcost i.
        // flag: whether it is in the tree

        // initialize the empty tree
        for (int i = 0; i < Vers; i++) {
            flag[i] = false;
            lowCost[i] = NoEdge;
        }

        // make 0 the first Node (or just choode randomly)
        int start = 0;

        // iterate:
        for (int i = 1; i < Vers; i++) {
            // for start is the new Node to be added into subset U, we need to update lowcost
            for (EdgeNode *p = verlist[start].head; p != nullptr; p = p->next) {
                if (flag[p->end] == false && lowCost[p->end] > p->weight) {
                    lowCost[p->end] = p->weight;
                    startNode[p->end] = start;
                }
            }
            // then we can find which edges and which vertex to put in!

            flag[start] = true;
            Edge min = NoEdge;

            // find the edge that has the minimum weight, update start
            for (int j = 0; j < Vers; j++) {
                if (lowCost[j] < min) {
                    min = lowCost[j];
                    start = j;
                }
            }

            std::cout << "(" << verlist[startNode[start]].value << "," << verlist[start].value << ")\n";
            lowCost[start] = NoEdge;
        }

        delete[] flag;
        delete[] startNode;
        delete[] lowCost;
    }
};

int main() {
    // Define the vertices of the graph
    char vertices[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    int numVertices = sizeof(vertices) / sizeof(vertices[0]);

    // Create the graph
    AdjListGraph<char, int> graph(numVertices, vertices);

    // Add edges to the graph (u, v, weight)
    graph.insert('A', 'B', 4);
    graph.insert('B', 'C', 6);
    graph.insert('C', 'D', 3);
    graph.insert('D', 'E', 2);
    graph.insert('E', 'F', 1);
    graph.insert('C', 'E', 4);
    graph.insert('A', 'F', 2);
    graph.insert('B', 'F', 5);

    // Perform Prim's algorithm to find the Minimum Spanning Tree (MST)
    std::cout << "Edges in the Minimum Spanning Tree (MST):" << std::endl;
    graph.Prim(INT_MAX);// Pass INT_MAX as the representation of "infinity"

    return 0;
}
