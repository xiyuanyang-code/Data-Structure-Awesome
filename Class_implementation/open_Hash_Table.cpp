#include <cstddef>
#include <iostream>
#include <queue>
#include <random>


template<class KEY, class OTHER>
struct set {
    KEY key;
    OTHER other;
};

// Dynamic Search Table
template<class Key, class Other>
class dynamicSearchTable {
public:
    // finding elements of x
    virtual set<Key, Other> *find(const Key &x) const = 0;

    // insert element, maintaining the order
    virtual void insert(const set<Key, Other> &x) = 0;

    // remove an element, maintaining the order
    virtual void remove(const Key &x) = 0;

    virtual ~dynamicSearchTable() {}
};

template<class Key, class Other>
class open_Hash_Table {
private:
    struct Node {
        set<Key, Other> data;
        Node *next;
        Node(const set<Key, Other> &d, Node *n = nullptr) {
            next = n;
            data = d;
        }
    };

    // like the chunking algorithms
    Node **array;
    size_t size;
    int (*key)(const Key &x);
    static int defaultKey(const int &x) {
        return x;
    }

public:
    open_Hash_Table(size_t size_, int (*f)(const Key &x) = defaultKey) {
        size = size_;
        key = f;
        array = new Node *[size];

        // initialize the array
        for (int i = 0; i < size; i++) {
            array[i] = nullptr;
        }
    }

    ~open_Hash_Table() {
        // delete all the array
        Node *p, *q;
        for (int i = 0; i < size; i++) {
            p = array[i];
            // delete the linked list of p
            while (p != nullptr) {
                q = p->next;
                delete p;
                p = p->next;
            }
        }
    }

    set<Key, Other> *find(const Key &x) const {
        int pos;
        Node *p;
        pos = key(x) % size;
        p = array[pos];
        while (p != nullptr && !(p->data.key == x)) {
            p = p->next;
        }

        if (p == nullptr) {
            return nullptr;
            // we don't find the element.
        } else {
            return (set<Key, Other> *) p;
        }
    }

    void insert(const set<Key, Other> &x) {
        int pos;
        Node *p;

        pos = key(x.key) % size;
        array[pos] = new Node(x, array[pos]);
    }

    void remove(const Key &x) {
        int pos;
        Node *p, *q;
        pos = key(x) % size;

        if (array[pos] == nullptr) {
            // no elements
            return;
        } else {
            p = array[pos];
            // search for the linked list of p

            // for the single element
            if (p->data.key == x) {
                array[pos] = p->next;
                delete p;
                return;
            } else {
                while (p->next != nullptr && !(p->next->data.key == x)) {
                    p = p->next;
                }

                if (p->next != nullptr) {
                    // we find the element
                    q = p->next;
                    p->next = q->next;
                    delete q;
                }
            }
        }
    }
};

int generate_random(int range = 100000) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, range);
    return dis(gen);
}

std::string generate_random_string(size_t length = 20) {
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, characters.size() - 1);

    std::string random_string;
    for (size_t i = 0; i < length; ++i) {
        random_string += characters[dis(gen)];
    }
    return random_string;
}


int main() {
    open_Hash_Table<int, std::string> s(10000);
    long long alldata_range = 1000000000;
    long long alldata_num = 8000;
    long long query_num = 1000000;
    set<int, std::string> element;
    std::queue<int> storage;

    for (int i = 0; i < alldata_num; i++) {
        set<int, std::string> element;
        element.key = generate_random(alldata_range);
        element.other = generate_random_string();
        s.insert(element);
        storage.push(element.key);
    }

    // for the searching period
    for (int i = 0; i < query_num; i++) {
        int search_key = generate_random(alldata_range);
        if (s.find(search_key) != nullptr) {
            std::cout << "Find the key value " << search_key << std::endl;
        }

        if (generate_random(1000) % 3 == 0 && !storage.empty()) {
            // randomly remove some elements
            int dele = storage.front();
            s.remove(dele);
            if (s.find(dele) == nullptr) {
                std::cout << "The key value " << dele << " has been deleted successfully." << std::endl;
            }
            storage.pop();
        }
    }

    return 0;
}
