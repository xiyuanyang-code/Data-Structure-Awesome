#include <algorithm>//just for the min function
#include <cassert>
#include <cmath>//for sqrt() function
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>//for KMP only

// Sequential implementation of string
class seqString {
    friend seqString operator+(const seqString &s1, const seqString &s2);
    friend bool operator==(const seqString &s1, const seqString &s2);
    friend bool operator!=(const seqString &s1, const seqString &s2);
    friend bool operator>(const seqString &s1, const seqString &s2);
    friend bool operator>=(const seqString &s1, const seqString &s2);
    friend bool operator<(const seqString &s1, const seqString &s2);
    friend bool operator<=(const seqString &s1, const seqString &s2);
    friend std::istream &operator>>(std::istream &is, seqString &s);
    friend std::ostream &operator<<(std::ostream &os, const seqString &s);

private:
    char *data;
    int len = 0;

public:
    seqString(const char *s = "");
    seqString(const seqString &other);
    ~seqString();
    seqString &operator=(const seqString &other);
    seqString &operator+=(const seqString &other);
    seqString &operator-=(const seqString &other);
    char operator[](int index) const;
    void insert(int start, const seqString &other, int insertsize = -1);
    void remove(int deletelength, int start = -1);
    int length() const;
    bool isEmpty() const;
    seqString substr(int start, int leng) const;
};

// Defintion of Sequenial String

// default constructer
seqString::seqString(const char *s) {
    // first get the value of length;
    while (s[len] != '\0') {
        len++;
    }
    data = new char[len + 1];
    for (int i = 0; s[i] != '\0'; i++) {
        data[i] = s[i];
    }
    data[len] = '\0';
}

// copy constructor
seqString::seqString(const seqString &other) {
    len = other.len;
    data = new char[len + 1];
    for (int i = 0; i <= len; i++) {
        data[i] = other.data[i];
    }
}

// destructor
seqString::~seqString() {
    delete[] data;
}

// overload of assignment operator
seqString &seqString::operator=(const seqString &other) {
    if (this == &other)
        return *this;
    delete[] data;
    len = other.len;
    data = new char[len + 1];
    for (int i = 0; i <= len; i++) {
        data[i] = other.data[i];
    }
    return *this;
}

// overload of "+=" operator
seqString &seqString::operator+=(const seqString &other) {
    this->insert(len, other);
    return *this;
}

// overload of "-=" operator
seqString &seqString::operator-=(const seqString &other) {
    seqString tmp = substr(len - other.length(), other.length());
    if (tmp == other) {
        this->remove(len - other.length(), other.length());
        return *this;
    } else {
        throw std::invalid_argument("The string cannot be removed!");
    }
}

// overload of index operator
char seqString::operator[](int index) const {
    if (index < 0 || index >= len) {
        throw std::runtime_error("Invalid index!");
    } else {
        return data[index];
    }
}

// insert a string in the certain position
void seqString::insert(int start, const seqString &other, int insertsize) {
    if (insertsize == -1) {
        // This means insert the whole size of other
        insertsize = other.length();
    }

    if (insertsize < 0 || insertsize > other.length()) {
        throw std::runtime_error("Invalid insert size!");
    } else if (start < 0 || start > len) {
        throw std::runtime_error("Invalid starting position!");
    } else {
        char *tmp = data;
        len += insertsize;
        data = new char[len + 1];
        for (int i = 0; i < start; i++) {
            data[i] = tmp[i];
        }
        for (int i = 0; i < insertsize; i++) {
            data[start + i] = other.data[i];
        }
        for (int i = start; tmp[i] != '\0'; i++) {
            data[i + insertsize] = tmp[i];
        }
        data[len] = '\0';
        delete[] tmp;
    }
}

// remove a substring from a certain position
void seqString::remove(int deletelength, int start) {
    if (start == -1) {
        // then delete the last leng characters from the back
        start = len - deletelength;
        // len represents the length of the whole string;
    }

    if (start < 0 || start >= len) {
        throw std::runtime_error("Invalid starting position!");
    }

    if (start + deletelength >= len) {
        // delete all the characters after the start index
        data[start] = '\0';
        len = start;
        char *tmp = data;
        // recycle the memory
        data = new char[len + 1];
        for (int i = 0; i <= len; i++) {
            data[i] = tmp[i];
        }
        delete[] tmp;
    } else {
        len -= deletelength;
        char *tmp = data;
        data = new char[len + 1];
        for (int i = 0; i < start; i++) {
            data[i] = tmp[i];
        }
        for (int i = start + deletelength; tmp[i] != '\0'; i++) {
            data[i - deletelength] = tmp[i];
        }
        data[len] = '\0';
        delete[] tmp;
    }
}

// get the length of the string
int seqString::length() const {
    return len;
}

// judge whether the string is empty
bool seqString::isEmpty() const {
    return len == 0;
}

// get a substring from a certain position and a certain length
seqString seqString::substr(int start, int sublength) const {
    if (start < 0 || start >= len) {
        throw std::runtime_error("Invalid starting position!");
    } else {
        if (sublength + start >= len) {
            sublength = len - start;
        }

        seqString tmp;// the target substring
        tmp.len = sublength;
        delete tmp.data;
        tmp.data = new char[tmp.len + 1];
        for (int i = 0; i < sublength; i++) {
            tmp.data[i] = data[start + i];
        }
        tmp.data[tmp.len] = '\0';
        return tmp;
    }
}

// overload of plus operation
seqString operator+(const seqString &s1, const seqString &s2) {
    seqString tmp;
    tmp.len = s1.len + s2.len;
    delete[] tmp.data;
    tmp.data = new char[tmp.len + 1];
    for (int i = 0; i < s1.len; i++) {
        tmp.data[i] = s1.data[i];
    }
    for (int i = 0; i < s2.len; i++) {
        tmp.data[i + s1.len] = s2.data[i];
    }
    tmp.data[tmp.len] = '\0';
    return tmp;
}

// several judgements
bool operator==(const seqString &s1, const seqString &s2) {
    if (s1.len != s2.len)
        return false;
    for (int i = 0; i < s1.len; i++) {
        if (s1.data[i] != s2.data[i])
            return false;
    }
    return true;
}

bool operator!=(const seqString &s1, const seqString &s2) {
    return !(s1 == s2);
}

bool operator>(const seqString &s1, const seqString &s2) {
    int minlength = std::min(s1.len, s2.len);
    for (int i = 0; i < minlength; i++) {
        if (s1[i] != s2[i])
            return s1[i] > s2[i];
    }
    return s1.len > s2.len;
}

bool operator>=(const seqString &s1, const seqString &s2) {
    return s1 > s2 || s1 == s2;
}

bool operator<(const seqString &s1, const seqString &s2) {
    return !(s1 >= s2);
}

bool operator<=(const seqString &s1, const seqString &s2) {
    return s1 < s2 || s1 == s2;
}

// the overload function of << and >> operator
std::istream &operator>>(std::istream &is, seqString &s) {
    const int bufferSize = 1024;
    char buffer[bufferSize];
    is >> buffer;

    int inputLen = std::strlen(buffer);

    if (s.data) {
        delete[] s.data;
    }

    s.data = new char[inputLen + 1];
    std::strcpy(s.data, buffer);
    s.len = inputLen;
    return is;
}

std::ostream &operator<<(std::ostream &os, const seqString &s) {
    os << s.data;
    return os;
}

/*
The linked implementation of string
*/

class linkString {
    friend linkString operator+(const linkString &s1, const linkString &s2);
    friend bool operator==(const linkString &s1, const linkString &s2);
    friend bool operator!=(const linkString &s1, const linkString &s2);
    friend bool operator>(const linkString &s1, const linkString &s2);
    friend bool operator>=(const linkString &s1, const linkString &s2);
    friend bool operator<(const linkString &s1, const linkString &s2);
    friend bool operator<=(const linkString &s1, const linkString &s2);
    friend std::istream &operator>>(std::istream &is, linkString &s);
    friend std::ostream &operator<<(std::ostream &os, const linkString &s);

    struct node {
        int size;// valuable numbers of chars in a block node
        char *data;
        node *next;

        // constructor
        node(int maxsize_ = 1, node *n_ = nullptr) {
            data = new char[maxsize_];
            size = 0;
            next = n_;
        }
    };

private:
    node *head;  // head pointer
    int length;  // length of the string
    int nodesize;// capacity for every block node

    // several private function tools
    void clear();                                     // release all the memories
    void findPos(int start, int &pos, node *&p) const;// find the position of the certain node
    void split(node *p, int pos);                     // split nodes
    void merge(node *p);                              // merge nodes

public:
    linkString(const char *s = "");                                      // Constructor with default parameter
    linkString(const linkString &other);                                 // Copy constructor
    ~linkString();                                                       // Destructor
    linkString &operator=(const linkString &other);                      // Assignment operator overload
    linkString &operator+=(const linkString &other);                     // Compound assignment operator overload (concatenation)
    linkString &operator-=(const linkString &other);                     // Compound assignment operator overload (removal)
    char operator[](int index) const;                                    // Subscript operator overload
    void insert(int start, const linkString &other, int insertsize = -1);// Insert operation
    void remove(int deletelength, int start = -1);                       // Remove operation
    int getlength() const;                                               // Get the length of the string
    bool isEmpty() const;                                                // Check if the string is empty
    linkString substr(int start, int sublength) const;                   // Get a substring
    void visualPrint() const;
};

// definition

// default constructor
linkString::linkString(const char *s) {
    // calculate the length of the string, you can just use strlen!
    length = std::strlen(s);
    nodesize = (length == 0) ? 1 : static_cast<int>(std::sqrt(length));
    node *p;
    p = head = new node(1);
    while (*s != '\0') {
        p = p->next = new node(nodesize);
        for (; (p->size < nodesize) && *s != '\0'; ++s, ++p->size) {
            p->data[p->size] = *s;
        }
    }
}

// copy constructor
linkString::linkString(const linkString &other) {
    node *p, *otherp = other.head->next;

    // copy the new linkstring
    p = head = new node(1);
    length = other.length;
    nodesize = other.nodesize;
    while (otherp != nullptr) {
        p = p->next = new node(nodesize);
        for (; (p->size) < (otherp->size); ++(p->size)) {
            // if p->size == other->size ,then all the valuable data has been copied successfully.
            p->data[p->size] = otherp->data[p->size];
        }
        otherp = otherp->next;// traverse the other link string
    }
}

/**
 * @brief clear all the memory of the linked list (private func)
 */
void linkString::clear() {
    node *p = head->next, *nextp;
    // nextp store next pointer temporarily
    while (p != nullptr) {
        nextp = p->next;
        delete p;
        p = nextp;
    }
}

/**
 * @brief destructor
 */
linkString::~linkString() {
    clear();
    delete head;
}

/**
 * @brief get the length of the string (public func)
 * @return the length of string
 */
int linkString::getlength() const {
    return length;
}

/**
 * @brief judge whether the string is empty
 */
bool linkString::isEmpty() const {
    return length == 0;
}

/**
 * @brief overload of assignment operator
 *
 * @param other the rvalue (const reference) of the assignment operator
 * @return the reference of the assigned lvalue
 */
linkString &linkString::operator=(const linkString &other) {
    if (&other == this)
        return *this;

    // Clear the current list
    clear();

    // Copy the length and nodesize
    length = other.length;
    nodesize = other.nodesize;

    // Copy the nodes
    node *p = head;
    node *otherp = other.head->next;

    while (otherp != nullptr) {
        p->next = new node(nodesize);
        p = p->next;

        // Copy the data
        for (int i = 0; i < otherp->size; ++i) {
            p->data[i] = otherp->data[i];
        }
        p->size = otherp->size;

        // Move to the next node in the other list
        otherp = otherp->next;
    }

    return *this;
}

/**
 * @brief find the starting position of a given position
 *
 * @param start the given position (or index)
 * @param pos find the position in the node (reference)
 * @param p find the node where the start lies in
 */
void linkString::findPos(int start, int &pos, node *&p) const {
    int count = 0; // define a counter
    p = head->next;// the first node(pointer)

    while (count < start) {
        if (count + p->size < start) {
            // the start isn't in this node!
            count += p->size;
            p = p->next;
        } else {
            // the start is in this node!
            pos = start - count;
            return;
        }
    }
}

/**
 * @brief get a substring
 *
 * @param start the start position
 * @param sublength the length of the substring
 * @throws std::out_of_range If pos is out of range.
 */
linkString linkString::substr(int start, int sublength) const {
    linkString tmp;// storing result
    int count = 0;
    int pos = 0;
    node *p, *tp = tmp.head;

    if (start < 0 || start >= length) {
        throw std::out_of_range("Invalid starting position!");
    } else {
        // if the length left is smaller than the given sublength:
        if (start + sublength >= length) {
            sublength = length - start;
        }

        tmp.length = sublength;
        tmp.nodesize = (sublength == 0) ? 1 : static_cast<int>(std::sqrt(sublength));

        // use the findPos function to find p and pos
        findPos(start, pos, p);

        // after getting the starting position, we can copy the substring!
        for (int index = 0; index < tmp.length;) {
            /*
             * two for-loop layers, index represents the substring index
             * inner loop is used to copy every block, while the outer loop is used to update the block.
             */
            tp = tp->next = new node(tmp.nodesize);
            while (tp->size <= tmp.nodesize && index < tmp.length) {
                if (pos == p->size) {
                    /*
                     * the traverse has ended in the block.
                     * p needs to be updated to the next block.
                     */
                    p = p->next;
                    pos = 0;
                }
                tp->data[tp->size] = p->data[pos];

                // value update
                pos++;
                index++;
                tp->size++;
            }
        }
        return tmp;
    }
}

/**
 * @brief Split operations for a block (one to two) (private func).
 *
 * @param p The node to be split.
 * @param pos The specific index where the split happens (the pos position is moved to the second node).
 * @throws std::out_of_range If pos is out of range.
 */
void linkString::split(node *p, int pos) {
    // Check if p is nullptr
    if (p == nullptr) {
        throw std::invalid_argument("The node to be split is null.");
    }

    // Check if pos is within the valid range
    if (pos < 0 || pos > p->size) {
        throw std::out_of_range("The split position is out of range.");
    }

    // Create a new node and insert it after p
    node *nextp = new node(nodesize, p->next);
    p->next = nextp;

    // Move data from p to nextp
    for (int i = pos; i < p->size; i++) {
        nextp->data[i - pos] = p->data[i];
    }

    // Update sizes of both nodes
    nextp->size = p->size - pos;// Size of the new node
    p->size = pos;              // Size of the original node after split
}

/**
 * @brief merge two nodes into one
 *
 * @param p one of the nodes that need to be merged
 */
void linkString::merge(node *p) {
    if (p == nullptr || p->next == nullptr)
        return;
    node *nextp = p->next;
    if (p->size + nextp->size <= nodesize) {
        // merge available
        for (int pos = 0; pos < nextp->size; ++pos, ++p->size) {
            p->data[p->size] = nextp->data[pos];
        }
        p->next = nextp->next;
        delete nextp;
    }
}

/**
 * @brief insert a string into linked string, using the findPos, split and merge functions
 *
 * @param start the starting position (using findPos)
 * @param other the string that needs to be inserted
 * @param insertsize the length of the string needs to be inserted, default value for all string
 */
void linkString::insert(int start, const linkString &other, int insertsize) {
    if (insertsize == -1) {
        insertsize = other.length;
    }

    if (start < 0 || start > length) {
        throw std::out_of_range("The insert position is out of range");
    }

    if (insertsize < 0 || insertsize > other.length) {
        throw std::out_of_range("Invalid insertsize");
    }

    /*
     * pos and p is calculated by func findPos, representing the insert position
     * nextp is right after p, which is used for the split func
     * after the insert, the merge function will be used
     */
    node *p, *nextp, *tmp;
    int pos = 0;
    findPos(start, pos, p);
    split(p, pos);

    nextp = p->next;// nextp is used for stroage in case the link string is broken
    linkString tobeinserted = other.substr(0, insertsize);
    tmp = tobeinserted.head->next;
    while (tmp != nullptr) {
        // operate for every node
        for (pos = 0; pos < tmp->size; ++pos) {
            if (p->size == nodesize) {
                // need for expansion
                p = p->next = new node(nodesize);
            }
            p->data[p->size] = tmp->data[pos];
            ++p->size;
        }
        tmp = tmp->next;
    }

    length += insertsize;
    p->next = nextp;
    merge(p);// see whether the merge is available
}

/**
 * @brief Remove the specific part of a linked string
 *
 * @param deletelength the length to be deleted
 * @param start the starting position that needs tobe removed (the start itself is included), default for remove from the back
 */
void linkString::remove(int deletelength, int start) {
    if (start == -1) {
        start = length - deletelength;
    }

    if (start < 0 || start >= length) {
        throw std::runtime_error("Invalid starting position!");
    }

    // find the position to be removed
    node *startp;// represent the starting position to be deleted
    int pos = 0;
    findPos(start, pos, startp);

    split(startp, pos);// split the node

    if (start + deletelength >= length) {
        deletelength = length - start;
        length = start;
        // if the deletelength goes over the edge
    } else {
        length -= deletelength;
    }

    while (true) {
        node *nextp = startp->next;
        if (deletelength > nextp->size) {
            // the end node is not here! Then delete this node!
            deletelength -= nextp->size;
            startp->next = nextp->next;
            delete nextp;
        } else {
            // the end node is here!
            split(nextp, deletelength);
            startp->next = nextp->next;
            // now the nextp is all the data needs tobe deleted, while nextp->next stores the data remaining
            delete nextp;
            break;// jump out of the loop after the remove operation is done!
        }
    }
    merge(startp);// merge all the startp;
}

/**
 * @brief Overloads the [] operator to access a character at a specific index in the linkString.
 *
 * @param index The position of the character to access (0-based index).
 * @return The character at the specified index.
 * @throws std::out_of_range If the index is invalid (less than 0 or greater than or equal to the string length).
 */
char linkString::operator[](int index) const {
    if (index < 0 || index >= length) {
        throw std::out_of_range("Index " + std::to_string(index) + " is out of range. Valid range is [0, " + std::to_string(length - 1) + "].");
    }

    node *current_node;   // Pointer to the node containing the target character
    int node_position = 0;// Position within the node
    findPos(index, node_position, current_node);

    return current_node->data[node_position];
}

/**
 * @brief the overload function of += operator
 *
 * @param other the linkString needs to be appended
 * @return the reference of the appended string
 */
linkString &linkString::operator+=(const linkString &other) {
    this->insert(length, other);
    return *this;
}

/**
 * @brief the overload function of -= operator
 *
 * @param other the linkString needs to be poped (removed)
 * @return the reference of the string
 */
linkString &linkString::operator-=(const linkString &other) {
    linkString tmp = substr(length - other.length, other.length);
    if (tmp == other) {
        this->remove(length - other.length, other.length);
        return *this;
    } else {
        throw std::invalid_argument("The string cannot be removed!");
    }
}

/**
 * @brief the overload function of + operator
 *
 * @param s1 the left plus string
 * @param s2 the right plus string
 *
 * @return a tmp value of the added string
 */
linkString operator+(const linkString &s1, const linkString &s2) {
    linkString ans = s1;
    ans += s2;
    return std::move(ans);
}

/**
 * @brief judge whether the two strings is equal
 *
 * @param s1 the left string needs to be judged
 * @param s2 the right string needs to be judged
 *
 * @return a bool value
 */
bool operator==(const linkString &s1, const linkString &s2) {
    if (s1.length != s2.length)
        return false;
    // uses two pointers to traverse the linkString

    linkString::node *p1 = s1.head->next;
    linkString::node *p2 = s2.head->next;
    int current_pos_for_s1 = 0;
    int current_pos_for_s2 = 0;

    while (p1 && p2) {
        // the end statement:one of the node* reacheds to the end
        if (p1->data[current_pos_for_s1] != p2->data[current_pos_for_s2]) {
            return false;
        }

        // update the index
        current_pos_for_s1++;
        current_pos_for_s2++;

        // update the node
        if (current_pos_for_s1 == p1->size) {
            p1 = p1->next;
            current_pos_for_s1 = 0;
        }

        if (current_pos_for_s2 == p2->size) {
            p2 = p2->next;
            current_pos_for_s2 = 0;
        }
    }
    return true;
}

/**
 * @brief judge whether the two strings are not equal
 * * All the parameters are equal to the == function
 */
bool operator!=(const linkString &s1, const linkString &s2) {
    return !(s1 == s2);
}

// several functions for string comparison

/**
 * @brief the sorted function for string comparison, if s1 > s2, then the func returns true.
 */
bool operator>(const linkString &s1, const linkString &s2) {
    // pointers for traverse
    linkString::node *p1 = s1.head->next;
    linkString::node *p2 = s2.head->next;
    int current_pos_for_s1 = 0;
    int current_pos_for_s2 = 0;

    while (p1 != nullptr) {
        if (p1->data[current_pos_for_s1] < p2->data[current_pos_for_s2]) {
            return false;
        }

        if (p1->data[current_pos_for_s1] > p2->data[current_pos_for_s2]) {
            return true;
        }

        // update the index
        current_pos_for_s1++;
        current_pos_for_s2++;

        // update the node
        if (current_pos_for_s1 == p1->size) {
            p1 = p1->next;
            current_pos_for_s1 = 0;
        }

        if (current_pos_for_s2 == p2->size) {
            p2 = p2->next;
            current_pos_for_s2 = 0;
        }
    }
    // whatever p2 is (nullptr or not), there is no possilbility s1>s2!
    return false;
}

bool operator>=(const linkString &s1, const linkString &s2) {
    return s1 > s2 || s1 == s2;
}

bool operator<(const linkString &s1, const linkString &s2) {
    return !(s1 >= s2);
}

bool operator<=(const linkString &s1, const linkString &s2) {
    return !(s1 > s2);
}

/**
 * @brief the overload function of << operator(for output)
 *
 * @param os reference to the istream classes
 * @param s the string that needs to be printed.
 *
 * @return the l-value for os
 */
std::ostream &operator<<(std::ostream &os, const linkString &s) {
    // for traverse
    linkString::node *p = s.head->next;

    while (p != nullptr) {
        for (int index = 0; index < p->size; index++) {
            os << p->data[index];
            // because the string doesnot have '\0', we cannot << it at a time.
        }
        p = p->next;
    }

    return os;
}

/**
 * @brief the overload function of >> operator(for input)
 *
 * @param is reference to the istream classes
 * @param s the string that needs to be printed.
 *
 * @return the l-value for is
 */
std::istream &operator>>(std::istream &is, linkString &s) {
    const int maxinputsize = 1024;
    char *newstring = new char[maxinputsize];
    int currrentlength = 0;
    newstring[0] = '\0';

    char current_ch;
    while (is.get(current_ch) && current_ch != '\n') {
        newstring[currrentlength] = current_ch;
        currrentlength++;
    }
    newstring[currrentlength] = '\0';

    linkString tmp(newstring);
    s = tmp;

    delete[] newstring;
    return is;
}

/**
 * @brief the visualization of linked string
 *
 */
void linkString::visualPrint() const {
    // for traverse
    node *current_node = head->next;
    int current_node_count = 0;

    std::cout << "The visualization for the linkString:" << std::endl;
    std::cout << "The string has " << length << "characters" << std::endl;

    while (current_node != nullptr) {
        std::cout << "Node " << current_node_count + 1 << ": " << std::endl;
        std::cout << "  Address: " << current_node << std::endl;
        std::cout << "  Size: " << current_node->size << std::endl;
        std::cout << "  Data: ";

        for (int i = 0; i < current_node->size; ++i) {
            std::cout << current_node->data[i];
        }
        std::cout << std::endl;

        std::cout << "----------------------------------------------------------------------" << std::endl;

        current_node = current_node->next;
        ++current_node_count;
    }
}

/*
 *The following code is for string-match problems
 *We use our handmade string for sequence implementation
 */
// declaration
void outermatch(seqString target, seqString totalString, int (*string_match_algorithm[])(seqString, seqString), int func_choice);
int complexStringMatch(seqString target, seqString totalString);
int KMP(seqString target, seqString totalString);
void computeLPS(const seqString &pattern, std::vector<int> &next);

/**
 * @brief The outer message output of string-matching problems
 *
 * @param target The target string needs to be matched
 * @param total_string The whole string that needs to be detected
 * @param string_match_algorithm The specific string-match problems
 * @param func_choice Choose one algorithm
 */
void outermatch(seqString target, seqString totalString, int (*string_match_algorithm[])(seqString, seqString), int func_choice) {
    int index = string_match_algorithm[func_choice](target, totalString);
    std::cout << "Using " << func_choice << "th algorithm" << std::endl;
    if (index == -1) {
        std::cout << "Unfortunately, the match fails." << std::endl;
        std::cout << "The string " << totalString << " does not have a substring " << target << std::endl;
    } else {
        std::cout << "Got it!" << std::endl;
        std::cout << "There exits a target string with the index of " << index << " and with the length of " << target.length() << std::endl;
        std::cout << "Which means there exists a substr: " << totalString.substr(index, target.length()) << " ,which matches with target: " << target << std::endl;
    }
}

/**
 * @brief The most complex implementation of string-matching, using enumeration
 *
 * @param target The target string needs to be matched
 * @param total_string The whole string that needs to be detected
 *
 * @return the index for the first match. return -1 if no match occurs.
 */
int complexStringMatch(seqString target, seqString totalString) {
    int target_length = target.length();
    int total_length = totalString.length();

    if (total_length < target_length) {
        return false;
    }

    for (int i = 0; i + target_length <= total_length; ++i) {
        seqString tmp = totalString.substr(i, target_length);

        if (tmp == target) {
            // match successfully
            return i;
        }
    }
    return -1;
}

/**
 * !KMP algorithm
 * @brief  the Knuth-Morris-Pratt (KMP) algorithm
 *
 * @param text The main text string to search within
 * @param pattern The pattern string to search for
 * @return The first occurrence index of the pattern in the text, or -1 if not found
 */
void computeLPS(const seqString &pattern, std::vector<int> &lps) {
    int len = 0;// Length of the previous longest prefix suffix
    lps[0] = 0; // lps[0] is always 0
    int i = 1;

    while (i < pattern.length()) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];// Fallback to the previous longest prefix suffix
            } else {
                lps[i] = 0;// No matching prefix suffix
                i++;
            }
        }
    }
}

// KMP algorithm implementation
int KMP(seqString pattern, seqString text) {
    if (pattern.isEmpty()) return 0;// Empty pattern matches at the start

    int textLen = text.length();
    int patLen = pattern.length();
    std::vector<int> lps(patLen, 0);
    computeLPS(pattern, lps);

    int i = 0;// Index for text
    int j = 0;// Index for pattern

    while (i < textLen) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == patLen) {
            return i - j;// Match found, return the starting index
        } else if (i < textLen && pattern[j] != text[i]) {
            if (j != 0) {
                j -= lps[j - 1];// Fallback using the LPS array
            } else {
                i++;// No fallback possible, move to the next character in text
            }
        }
    }

    return -1;// No match found
}

/*
 *The Debugging ToolBox
 */
// declaration
void testPackageForSeqString();
void testPackageForLinkString();
void testPackageForKMP();
void basicTestForSeqString();
void basicTestForLinkString();
void edgeCaseTestForLinkString();

void basicTestForSeqString() {
    seqString s1("Hello");
    seqString s2("World");

    // Test operator+
    seqString s3 = s1 + s2;
    std::cout << "s1 + s2: " << s3 << std::endl;

    // Test operator+=
    s1 += s2;
    std::cout << "s1 += s2: " << s1 << std::endl;

    // Test operator-
    s1 -= s2;
    std::cout << "s1 -= s2: " << s1 << std::endl;

    // Test operator[]
    std::cout << "s1[1]: " << s1[1] << std::endl;

    // Test insert
    s1.insert(1, s2);
    std::cout << "s1 after insert: " << s1 << std::endl;

    // Test remove
    s1.remove(5, 1);
    std::cout << "s1 after remove: " << s1 << std::endl;

    // Test substr
    seqString s4 = s1.substr(1, 3);
    std::cout << "s1.substr(1, 3): " << s4 << std::endl;

    // Test comparison operators
    std::cout << "s1 == s2: " << (s1 == s2) << std::endl;
    std::cout << "s1 != s2: " << (s1 != s2) << std::endl;
    std::cout << "s1 > s2: " << (s1 > s2) << std::endl;
    std::cout << "s1 < s2: " << (s1 < s2) << std::endl;
}

void basicTestForLinkString() {

    // Basic constructor test
    linkString s1("Hello");
    std::cout << "s1: " << s1 << " (Length: " << s1.getlength() << ")\n";
    s1.visualPrint();

    // Copy constructor test
    linkString s2 = s1;
    std::cout << "\nCopied s2: " << s2 << std::endl;
    assert(s1 == s2);

    // Assignment operator test
    linkString s3;
    s3 = s1;
    std::cout << "Assigned s3: " << s3 << std::endl;
    assert(s3 == s1);

    // Concatenation test
    s1 += " World";
    std::cout << "\nAfter +=: " << s1 << std::endl;
    assert(s1.getlength() == 11);

    // Substring test
    linkString sub_str = s1.substr(6, 5);
    std::cout << "Substr(6,5): " << sub_str << std::endl;
    assert(sub_str == "World");

    // Insertion test
    s1.insert(5, " C++");
    std::cout << "\nAfter insert: " << s1 << std::endl;
    assert(s1.getlength() == 15);

    // Deletion test
    s1.remove(4, 5);
    std::cout << "After remove: " << s1 << std::endl;
    assert(s1.getlength() == 11);

    // Comparison test
    linkString cmp("Hello World");
    std::cout << "\nComparison: " << (s1 > cmp ? "GT" : (s1 < cmp ? "LT" : "EQ")) << std::endl;
}

void edgeCaseTestForLinkString() {
    // Empty string test
    linkString empty;
    assert(empty.isEmpty());

    // Single character test
    linkString single("A");
    single.visualPrint();

    // Cross-node operation test
    linkString longStr("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    std::cout << "\nLong string nodes:\n";
    longStr.visualPrint();

    // Boundary deletion test
    longStr.remove(25, 0);
    assert(longStr.getlength() == 1);
}

void testPackageForSeqString() {
    std::cout << "Assert for the SeqString " << std::endl;
    try {
        basicTestForSeqString();
        std::cout << "\nAll tests passed!" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void testPackageForLinkString() {
    std::cout << "Assert for the linkString " << std::endl;
    try {
        basicTestForLinkString();
        edgeCaseTestForLinkString();
        std::cout << "\nAll tests passed!" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void testPackageForKMP() {
    seqString s1("Hello my name is YXY and I love learning DS");
    seqString targets[] = {"Hello", "my", "MM"};
    int (*string_match_algorithm[])(seqString, seqString) = {complexStringMatch, KMP};

    for (int i = 0; i < 2; i++) {
        for (auto target : targets) {
            outermatch(target, s1, string_match_algorithm, i);
        }
    }
}

inline void divisionline() {
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}

// Main function for debugging
int main() {
    divisionline();
    testPackageForLinkString();
    divisionline();

    divisionline();
    testPackageForSeqString();
    divisionline();

    divisionline();
    testPackageForKMP();
    divisionline();
    return 0;
}