#include <cstddef>
#include <iostream>
#include <queue>

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

    struct Edge_Kruskal {
        int begin, end;
        Edge w;

        Edge_Kruskal(int begin_, int end_, Edge w_) {
            begin = begin_;
            end = end_;
            w = w_;
        }

        // !attention, we need to construct a minimum heap
        bool operator<(const Edge_Kruskal &other) const {
            return w > other.w;
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
        ++Edges;
    }

    // other functions of a basic graph skipped

    void Kruskal() const {
        int edges_accepted = 0;
        int u, v;
        disjointSet ds(Vers);
        std::priority_queue<Edge_Kruskal> pq;

        // generating the pq
        for (int i = 0; i < Vers; i++) {
            for (EdgeNode *p = verlist[i].head; p != nullptr; p = p->next) {
                if (i < p->end) {
                    // ensure all edges will be added into pq only once
                    Edge_Kruskal added(i, p->end, p->weight);
                    pq.push(added);
                }
            }
        }

        // merging
        while (edges_accepted < Vers - 1) {
            Edge_Kruskal output = pq.top();
            pq.pop();
            int u = ds.Find(output.begin);
            int v = ds.Find(output.end);
            if (u != v) {
                // add into tree
                edges_accepted++;
                ds.Union(u, v);
                std::cout << "(" << verlist[output.begin].value << "," << verlist[output.end].value << ")" << std::endl;
            }
        }
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
    graph.insert('A', 'F', 2);
    graph.insert('B', 'C', 6);
    graph.insert('B', 'F', 5);
    graph.insert('C', 'D', 3);
    graph.insert('C', 'E', 4);
    graph.insert('D', 'E', 2);
    graph.insert('E', 'F', 1);

    // Perform Kruskal's algorithm to find the Minimum Spanning Tree (MST)
    std::cout << "Edges in the Minimum Spanning Tree (MST):" << std::endl;
    graph.Kruskal();

    return 0;
}