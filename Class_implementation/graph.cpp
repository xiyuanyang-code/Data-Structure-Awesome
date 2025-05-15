#include <climits>
#include <iostream>
#include <string>

class exception {
protected:
    const std::string variant = "";
    std::string detail = "";

public:
    exception() {
    }
    exception(const exception &ec) : variant(ec.variant), detail(ec.detail) {
    }
    virtual std::string what() {
        return variant + " " + detail;
    }
};

class index_out_of_bound : public exception {
    /* __________________________ */
};

class runtime_error : public exception {
    /* __________________________ */
};

class invalid_iterator : public exception {
    /* __________________________ */
};

class container_is_empty : public exception {
    /* __________________________ */
};

template<class Ver, class Edge>
class graph {
public:
    virtual void insert(Ver x, Ver y, Edge weight) = 0;
    virtual void remove(Ver x, Ver y) = 0;
    virtual bool exist(Ver x, Ver y) const = 0;
};


// implementation of adjMatrix
// ! it is the directed graph
template<class Ver, class Edge>
class adjMatrix : public graph<Ver, Edge> {
private:
    // to store the matrix
    Edge **edge_matrix;

    // store the value of vertices
    Ver *ver_array;

    // to store \infty value (use INT_MAX)
    Edge noEdge;

    int num_vers;
    int num_edges;

private:
    // several private functions
    /**
     * @brief find index in ver_array for the sepcific vertices (0-based)
     * 
     * @param target 
     * @return int 
     */
    int find(Ver target) const {
        for (int i = 0; i < num_vers; i++) {
            if (ver_array[i] == target) {
                return i;
            }
        }
        throw invalid_iterator();
    }

public:
    // public functions
    adjMatrix(int vsize, const Ver *d, const Edge noEdge_ = INT_MAX) {
        num_vers = vsize;
        noEdge = noEdge_;
        num_edges = 0;

        // initialize several arrays
        ver_array = new Ver[num_vers];
        for (int i = 0; i < num_vers; i++) {
            ver_array[i] = d[i];
        }

        // initialize matrix, all \infty
        edge_matrix = new Edge *[num_vers];
        for (int i = 0; i < num_vers; i++) {
            edge_matrix[i] = new Edge[num_vers];
            for (int j = 0; j < num_vers; j++) {
                edge_matrix[i][j] = noEdge;
            }
        }
    }

    ~adjMatrix() {
        delete[] ver_array;
        for (int i = 0; i < num_vers; i++) {
            delete[] edge_matrix[i];// FIX: use delete[]
        }
        delete[] edge_matrix;// Also free the array of pointers itself
    }

    int num_ver() const {
        return num_vers;
    }
    int num_edge() const {
        return num_edges;
    }

    /**
     * @brief insert one edges into the graph
     * 
     * @param x 
     * @param y 
     * @param w 
     */
    void insert(Ver x, Ver y, Edge w) {
        int u = find(x);
        int v = find(y);

        edge_matrix[u][v] = w;
        // !attention: it is the directed graph, from u to v
        ++num_edges;
    }

    /**
     * @brief remove one edges out of the graph
     * 
     * @param x 
     * @param y 
     */
    void remove(Ver x, Ver y) {
        int u = find(x);
        int v = find(y);

        edge_matrix[u][v] = noEdge;
        --num_edges;
    }

    /**
     * @brief find whether an edge from x to y exists
     * 
     * @param x 
     * @param y 
     * @return true the directed edges exists
     * @return false the directed edges exists
     */
    bool exist(Ver x, Ver y) const {
        int u = find(x);
        int v = find(y);
        if (edge_matrix[u][v] == noEdge) {
            return false;
        }
        return true;
    }
};

template<class Ver, class Edge>
class adjListGrapgh : public graph<Ver, Edge> {
private:
    struct edge_node {
        // node for edges

        // index for the second endpoint
        int end;
        Edge weight;
        edge_node *next;

        edge_node(int e, Edge weight_, edge_node *next_ = nullptr) : end(e), weight(weight_), next(next_) {}
    };

    struct ver_node {
        // node for vertices
        Ver value;
        edge_node *head;
        ver_node(Ver value_ = Ver(), edge_node *head_ = nullptr) {
            value = value_;// FIX: assign value
            head = head_;
        }
    };

    ver_node *ver_array;
    int num_vers;
    int num_edges;

private:
    int find(Ver v) const {
        for (int i = 0; i < num_vers; i++) {
            if (ver_array[i].value == v) {
                return i;
            }
        }
        throw invalid_iterator();
    }

public:
    /**
     * @brief Construct a new adj List Grapgh object
     * 
     * @param size 
     * @param d 
     */
    adjListGrapgh(int size, const Ver *d) {
        num_vers = size;
        num_edges = 0;

        ver_array = new ver_node[num_vers];
        for (int i = 0; i < num_vers; i++) {
            ver_array[i] = d[i];
        }
    }

    /**
     * @brief Destroy the adj List Grapgh object
     * 
     */
    ~adjListGrapgh() {
        edge_node *current;
        for (int i = 0; i < num_vers; i++) {
            current = ver_array[i].head;// FIX: assign current
            while (current != nullptr) {
                edge_node *next = current->next;
                delete current;
                current = next;
            }
        }
        delete[] ver_array;
    }
    int num_ver() const {
        return num_vers;
    }
    int num_edge() const {
        return num_edges;
    }

    void insert(Ver x, Ver y, Edge w) {
        int u = find(x);
        int v = find(y);
        ver_array[u].head = new edge_node(v, w, ver_array[u].head);
        ++num_edges;
    }

    /**
     * @brief remove the edges from u to v
     * 
     * @param x 
     * @param y 
     */
    void remove(Ver x, Ver y) {
        int u = find(x);
        int v = find(y);
        edge_node *p = ver_array[u].head;

        if (p == nullptr) {
            // no edges
            return;
        }

        if (p->end == v) {
            // the first is the edge to be deleted
            ver_array[u].head = p->next;
            delete p;
            --num_edges;
            return;
        }

        while (p->next != nullptr && p->next->end != v) {
            p = p->next;
        }

        if (p->next != nullptr) {
            // p->next is the node to be deleted
            edge_node *q = p->next;
            p->next = q->next;
            delete q;
            --num_edges;
        }
    }

    bool exist(Ver x, Ver y) const {
        int u = find(x);
        int v = find(y);
        edge_node *p = ver_array[u].head;
        while (p != nullptr && p->end != v) {
            p = p->next;
        }
        if (p == nullptr) {
            return false;
        }
        return true;
    }
};


int main() {
    // Define vertices
    char vertices[] = {'A', 'B', 'C', 'D'};
    int numVertices = sizeof(vertices) / sizeof(vertices[0]);

    // Test adjMatrix
    adjMatrix<char, int> g(numVertices, vertices);

    g.insert('A', 'B', 5);
    g.insert('A', 'C', 3);
    g.insert('B', 'D', 2);

    std::cout << "adjMatrix:" << std::endl;
    std::cout << "Edge A->B exists? " << (g.exist('A', 'B') ? "Yes" : "No") << std::endl;
    std::cout << "Edge B->A exists? " << (g.exist('B', 'A') ? "Yes" : "No") << std::endl;
    std::cout << "Edge A->C exists? " << (g.exist('A', 'C') ? "Yes" : "No") << std::endl;
    std::cout << "Edge B->D exists? " << (g.exist('B', 'D') ? "Yes" : "No") << std::endl;

    g.remove('A', 'B');
    std::cout << "After removing A->B, exists? " << (g.exist('A', 'B') ? "Yes" : "No") << std::endl;
    std::cout << "Number of vertices: " << g.num_ver() << std::endl;
    std::cout << "Number of edges: " << g.num_edge() << std::endl;

    // Test adjListGrapgh
    adjListGrapgh<char, int> gl(numVertices, vertices);

    gl.insert('A', 'B', 5);
    gl.insert('A', 'C', 3);
    gl.insert('B', 'D', 2);

    std::cout << "\nadjListGrapgh:" << std::endl;
    std::cout << "Edge A->B exists? " << (gl.exist('A', 'B') ? "Yes" : "No") << std::endl;
    std::cout << "Edge B->A exists? " << (gl.exist('B', 'A') ? "Yes" : "No") << std::endl;
    std::cout << "Edge A->C exists? " << (gl.exist('A', 'C') ? "Yes" : "No") << std::endl;
    std::cout << "Edge B->D exists? " << (gl.exist('B', 'D') ? "Yes" : "No") << std::endl;

    gl.remove('A', 'B');
    std::cout << "After removing A->B, exists? " << (gl.exist('A', 'B') ? "Yes" : "No") << std::endl;
    std::cout << "Number of vertices: " << gl.num_ver() << std::endl;
    std::cout << "Number of edges: " << gl.num_edge() << std::endl;

    return 0;
}