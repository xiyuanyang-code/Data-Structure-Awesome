#include <climits>
#include <iostream>
#include <string>
#include <vector>

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

    struct Eulerian_Node {
        int Node_num;
        Eulerian_Node *next;
        Eulerian_Node(int ver) {
            Node_num = ver;
            next = nullptr;
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

    void EulerCircuit(int start, Eulerian_Node *&beg, Eulerian_Node *&end) {
        // initialize
        beg = end = new Eulerian_Node(start);
        while (ver_array[start].head != nullptr) {
            int next_node = ver_array[start].head->end;

            // !attention, it is the undirected graph
            remove(start, next_node);
            remove(next_node, start);
            start = next_node;
            end->next = new Eulerian_Node(start);
            end = end->next;
        }
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
                edge_node *next_ = current->next;
                delete current;
                current = next_;
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

    // !Add new Function: Eulerian path
    std::vector<int> EulerCurcuit(Ver start) {
        std::vector<int> ans;
        if (num_edges == 0) {
            // absolutely there is no path
            // return the empty vector
            return ans;
        }

        for (int i = 0; i < num_vers; i++) {
            int num_of_degree = 0;
            edge_node *r = ver_array[i].head;
            while (r != nullptr) {
                ++num_of_degree;
                r = r->next;
            }

            if (num_of_degree % 2 != 0) {
                // no exists!
                return ans;
            }
        }

        int start_pos = find(start);

        // clone a list of adjlist
        ver_node *tmp = clone();
        Eulerian_Node *beg, *end;
        EulerCircuit(start_pos, beg, end);
        while (true) {
            Eulerian_Node *current = beg;
            while (current->next != nullptr) {
                // check the afterward nodes of current
                if (ver_array[current->next->Node_num].head != nullptr) {
                    break;
                } else {
                    current = current->next;
                }
            }

            // all the edges have been visited
            if (current->next == nullptr) {
                break;
            }

            Eulerian_Node *q = current->next;
            // q is the node yet to be visited

            // find a subcircle and connect them
            Eulerian_Node *tb, *te;
            EulerCircuit(q->Node_num, tb, te);
            te->next = q->next;
            current->next = tb;
        }


        delete[] ver_array;
        ver_array = tmp;


        Eulerian_Node *tmp_store;
        while (beg != nullptr) {
            ans.push_back(ver_array[beg->Node_num].value);
            tmp_store = beg;
            beg = beg->next;
            delete tmp_store;
        }
        return ans;
    }

    ver_node* clone() const {
        ver_node *tmp = new ver_node[num_vers];
        edge_node *p;

        for (int i = 0; i < num_vers; i++) {
            tmp[i].value = ver_array[i].value;
            p = ver_array[i].head;
            while (p != nullptr) {
                tmp[i].head = new edge_node(p->end, p->weight, tmp[i].head);
                p = p->next;
            }
        }
        return tmp;
    }
};

int main() {
    // Example: Eulerian circuit in an undirected graph
    // Vertices: 0, 1, 2, 3
    // Edges: 0-1, 1-2, 2-0, 0-3, 3-2 (all degrees are even)
    int vertices[] = {0, 1, 2, 3, 4, 5};
    int numVertices = sizeof(vertices) / sizeof(vertices[0]);
    adjListGrapgh<int, int> g(numVertices, vertices);

    // Insert undirected edges (add both directions)
    g.insert(0, 1, 1);
    g.insert(1, 0, 1);
    g.insert(1, 2, 1);
    g.insert(2, 1, 1);
    g.insert(2, 0, 1);
    g.insert(0, 2, 1);
    g.insert(0, 3, 1);
    g.insert(3, 0, 1);
    g.insert(3, 2, 1);
    g.insert(2, 3, 1);
    g.insert(1, 4, 2);
    g.insert(4, 1, 2);
    g.insert(1, 5, 6);
    g.insert(5, 1, 6);
    g.insert(0, 4, 5);
    g.insert(4, 0, 5);
    g.insert(2, 5, 3);
    g.insert(5, 2, 3);

    std::vector<int> path = g.EulerCurcuit(0);

    if (path.empty()) {
        std::cout << "No Eulerian circuit exists in the graph." << std::endl;
    } else {
        std::cout << "Eulerian circuit: ";
        for (int v : path) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}