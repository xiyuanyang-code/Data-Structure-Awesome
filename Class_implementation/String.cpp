#include <stdexcept>
#include <iostream>
#include <algorithm> //just for the min function
#include <cstring>
#include <cmath>  //for sqrt() function


//Sequential implementation of string
class seqString{
    friend seqString operator+(const seqString& s1, const seqString& s2);
    friend bool operator==(const seqString& s1, const seqString& s2);
    friend bool operator!=(const seqString& s1, const seqString& s2);
    friend bool operator>(const seqString& s1, const seqString& s2); 
    friend bool operator>=(const seqString& s1, const seqString& s2);
    friend bool operator<(const seqString& s1, const seqString& s2);
    friend bool operator<=(const seqString& s1, const seqString& s2);
    friend std::istream& operator>>(std::istream& is,  seqString& s);
    friend std::ostream& operator<<(std::ostream& os, const seqString& s);
    
    private:
        char *data;
        int len = 0;

    public:
        seqString(const char* s="");     
        seqString(const seqString& other);   
        ~seqString();
        seqString& operator=(const seqString& other);
        seqString& operator+=(const seqString& other);
        seqString& operator-=(const seqString& other);
        char operator[](int index) const;
        void insert(int start, const seqString& other, int insertsize = -1);
        void remove(int deletelength, int start =-1);
        int length() const;
        bool isEmpty() const;
        seqString substr(int start, int leng) const;
};

//Defintion of Sequenial String

//default constructer
seqString::seqString(const char*s){
    //first get the value of length;
    while(s[len] != '\0'){
        len++;
    }
    data = new char [len+1];
    for(int i = 0; s[i] != '\0'; i++){
        data[i] = s[i];
    }
    data[len] = '\0';
}

//copy constructor
seqString::seqString(const seqString& other){
    len = other.len;
    data = new char [len+1];
    for(int i = 0; i <= len; i++){
        data[i] = other.data[i];
    }
}

//destructor
seqString::~seqString(){
    delete[] data;
}

//overload of assignment operator
seqString& seqString::operator=(const seqString& other){
    if (this == &other) return *this;
    delete[] data;
    len = other.len;
    data = new char [len+1];
    for(int i = 0; i <= len; i++){
        data[i] = other.data[i];
    }
    return *this;
}

//overload of "+=" operator
seqString& seqString::operator+=(const seqString& other){
    this -> insert(len, other);
    return *this;
}

//overload of "-=" operator
seqString& seqString::operator-=(const seqString& other){
    seqString tmp = substr(len - other.length(), other.length());
    if (tmp == other){
        this -> remove(len - other.length(), other.length());
        return *this;
    }else{
        throw std::runtime_error("Unmatched string");
    }
}

//overload of index operator
char seqString::operator[](int index) const{
    if(index < 0 || index >= len){
        throw std::runtime_error("Invalid index!");
    }else{
        return data[index];
    }
}

//insert a string in the certain position
void seqString::insert(int start, const seqString& other, int insertsize){
    if(insertsize == -1){
        //This means insert the whole size of other
        insertsize = other.length();
    }

    if( insertsize < 0 || insertsize > other.length()){
        throw std::runtime_error("Invalid insert size!");
    }
    else if( start < 0 || start > len){
        throw std::runtime_error("Invalid starting position!");
    }
    else{
        char *tmp = data;
        len += insertsize;
        data = new char [len+1];
        for(int i = 0; i < start; i++){
            data[i] = tmp[i];
        }
        for(int i = 0; i< insertsize; i++){
            data[start + i] = other.data[i];
        }
        for(int i = start; tmp[i] !='\0'; i++){
            data[i + insertsize] = tmp[i];
        }
        data[len] = '\0';
        delete[] tmp;
    }
}

//remove a substring from a certain position
void seqString::remove(int deletelength,int start ){
    if(start == -1){
        //then delete the last leng characters from the back
        start = len - deletelength;
        //len represents the length of the whole string;
    }

    if(start < 0 || start >= len){
        throw std::runtime_error("Invalid starting position!");
    }

    if(start + deletelength >= len){
        //delete all the characters after the start index
        data [start] = '\0';
        len = start;
        char * tmp = data;
        //recycle the memory
        data = new char [len+1];
        for(int i = 0; i<= len; i++){
            data[i] = tmp[i];
        }
        delete[] tmp;
    }else{
        len -= deletelength;
        char *tmp = data;
        data = new char [len+1];
        for(int i = 0; i < start; i++){
            data[i] = tmp[i];
        }
        for(int i = start + deletelength; tmp[i] != '\0'; i++){
            data[i - deletelength] = tmp[i];
        }
        data[len] = '\0';
        delete[] tmp;
    }
}

//get the length of the string
int seqString::length() const{
    return len;
}

//judge whether the string is empty
bool seqString::isEmpty() const{
    return len == 0;
}

//get a substring from a certain position and a certain length
seqString seqString::substr(int start, int sublength) const{
    if(start < 0 || start >= len){
        throw std::runtime_error("Invalid starting position!");
    }else{
        if(sublength + start >= len){
            sublength = len - start;
        }

        seqString tmp; //the target substring
        tmp.len = sublength;
        delete tmp.data;
        tmp.data = new char[tmp.len + 1];
        for(int i = 0; i < sublength; i++){
            tmp.data[i] = data[start + i];
        }
        tmp.data[tmp.len] = '\0';
        return tmp;
    }
}

//overload of plus operation
seqString operator+(const seqString& s1, const seqString& s2){
    seqString tmp;
    tmp.len = s1.len + s2.len;
    delete[] tmp.data;
    tmp.data = new char[tmp.len + 1];
    for(int i = 0; i < s1.len; i++){
        tmp.data[i] = s1.data[i];
    }
    for(int i = 0; i < s2.len; i++){
        tmp.data[i + s1.len] = s2.data[i];
    }
    tmp.data[tmp.len] = '\0';
    return tmp;
}

//several judgements
bool operator==(const seqString& s1, const seqString& s2){
    if(s1.len != s2.len) return false;
    for(int i = 0; i< s1.len; i++){
        if(s1.data[i] != s2.data[i]) return false;
    }
    return true;
}

bool operator!=(const seqString& s1, const seqString& s2){
    return !(s1==s2);
}

bool operator>(const seqString& s1, const seqString& s2){
    int minlength = std::min(s1.len, s2.len);
    for(int i = 0; i < minlength; i++){
        if(s1[i] != s2[i]) return s1[i] > s2[i];
    }
    return s1.len > s2.len;
}

bool operator>=(const seqString& s1, const seqString& s2){
    return s1 > s2 || s1 == s2;
}

bool operator<(const seqString& s1, const seqString& s2){
    return !(s1 >= s2);
}

bool operator<=(const seqString& s1, const seqString& s2){
    return s1 < s2 || s1 ==s2;
}

//the overload function of << and >> operator
std::istream& operator>>(std::istream& is, seqString& s) {
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

std::ostream& operator<<(std::ostream& os, const seqString& s){
    os << s.data;
    return os;
}





/*
The linked implementation of string
*/

class linkString{
    friend linkString operator+(const linkString& s1, const linkString& s2);
    friend bool operator==(const linkString& s1, const linkString& s2);
    friend bool operator!=(const linkString& s1, const linkString& s2);
    friend bool operator>(const linkString& s1, const linkString& s2); 
    friend bool operator>=(const linkString& s1, const linkString& s2);
    friend bool operator<(const linkString& s1, const linkString& s2);
    friend bool operator<=(const linkString& s1, const linkString& s2);
    friend std::istream& operator>>(std::istream& is,  linkString& s);
    friend std::ostream& operator<<(std::ostream& os, const linkString& s);

    struct node
    {
        int size;    //valuable numbers of chars in a block node
        char *data;
        node *next;

        //constructor
        node(int maxsize_ = 1, node *n_ = nullptr){
            data = new char [maxsize_];
            size = 0;
            next = n_;
        }
    };

    private:
        node *head; //head pointer
        int length;  //length of the string
        int nodesize; //capacity for every block node

        //several private function tools
        void clear();  //release all the memories
        void findPos(int start, int& pos, node *&p) const; // find the position of the certain node
        void split(node *p, int pos);  //split nodes
        void merge(node *p);  //merge nodes

    public:
        linkString(const char* s = "");          // Constructor with default parameter
        linkString(const linkString& other);     // Copy constructor
        ~linkString();                           // Destructor
        linkString& operator=(const linkString& other);  // Assignment operator overload
        linkString& operator+=(const linkString& other); // Compound assignment operator overload (concatenation)
        linkString& operator-=(const linkString& other); // Compound assignment operator overload (removal)
        char operator[](int index) const;                // Subscript operator overload
        void insert(int start, const linkString& other, int insertsize = -1); // Insert operation
        void remove(int deletelength, int start = -1);   // Remove operation
        int getlength() const;                              // Get the length of the string
        bool isEmpty() const;                            // Check if the string is empty
        linkString substr(int start, int sublength) const;    // Get a substring
};

//definition

//default constructor
linkString::linkString(const char *s){
    //calculate the length of the string, you can just use strlen!
    length = std::strlen(s);
    nodesize = (length == 0) ? 1 : static_cast<int>(std::sqrt(length));
    node *p;
    p = head = new node(1);
    while ( s != '\0'){
        p = p->next = new node(nodesize);
        for(; p->size < nodesize && s != '\0'; ++s, ++p->size){
            p->data[p->size] = *s;
        }
    }
}

//copy constructor
linkString::linkString(const linkString& other){
    node *p, *otherp = other.head -> next;

    //copy the new linkstring
    p = head = new node(1);
    length = other.length;
    nodesize = other.nodesize;
    while(otherp != nullptr){
        p = p->next = new node (nodesize);
        for(; p->size < otherp->size; ++p->size){
            //if p->size == other->size ,then all the valuable data has been copied successfully.
            p->data[p->size] = otherp->data[p->size];
        }
        otherp = otherp -> next; //traverse the other link string
    }
}

/**
 * @brief clear all the memory of the linked list (private func)
 */
void linkString::clear(){
    node *p = head ->next, *nextp;
    //nextp store next pointer temporarily
    while (p != nullptr){
        nextp = p ->next;
        delete p;
        p = nextp;
    }
}

/**
 * @brief destructor
 */
linkString::~linkString(){
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
bool linkString::isEmpty() const{
    return length == 0;
}

/**
 * @brief overload of assignment operator
 * 
 * @param other the rvalue (const reference) of the assignment operator
 * @return the reference of the assigned lvalue
 */
linkString& linkString::operator=(const linkString& other){
    if(&other == this) return *this;
    node *p = head;
    node *otherp = other.head->next;

    clear(); //clear all the memory of the previous string except for the head node.
    length = other.length;
    nodesize = other.nodesize;
    while (otherp != nullptr){
        p = p->next = new node [nodesize];
        for(; p->size < otherp->size; p->size++){
            p->data[p->size] = otherp->data[p->size];
        }
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
void linkString::findPos(int start, int& pos, node *&p) const{
    int count = 0;//define a counter
    p = head -> next;//the first node(pointer)
    
    while(count < start){
        if(count + p->size < start){
            //the start isn't in this node!
            count += p->size;
            p = p->next;
        }else{
            //the start is in this node!
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
linkString linkString::substr(int start, int sublength) const{
    linkString tmp; //storing result
    int count = 0;
    int pos = 0;
    node *p, *tp = tmp.head;

    if(start < 0 || start >= length){
        throw std::out_of_range("Invalid starting position!");
    }else{
        // if the length left is smaller than the given sublength:
        if(start + sublength >= length){
            sublength = length - start;
        }

        tmp.length = sublength;
        tmp.nodesize = (sublength == 0) ? 1 : static_cast<int>(std::sqrt(sublength));

        //use the findPos function to find p and pos
        findPos(start, pos, p);

        //after getting the starting position, we can copy the substring!
        for(int index = 0; index < tmp.length;){
            /*
             * two for-loop layers, index represents the substring index
             * inner loop is used to copy every block, while the outer loop is used to update the block.
             */
            tp = tp->next = new node(tmp.nodesize);
            while(tp->size <= tmp.nodesize && index < tmp.length){
                if(pos == p->size){
                    /*
                     * the traverse has ended in the block.
                     * p needs to be updated to the next block.
                    */
                    p = p->next;
                    pos = 0;
                }
                tp->data[tp->size] = p->data[pos];

                //value update
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
    nextp->size = p->size - pos; // Size of the new node
    p->size = pos;               // Size of the original node after split
}


/**
 * @brief merge two nodes into one
 * 
 * @param p one of the nodes that need to be merged 
 */
void linkString::merge(node *p){
    if(p == nullptr || p->next == nullptr) return;
    node *nextp = p->next;
    if(p->size + nextp->size <= nodesize){
        //merge available
        for(int pos = 0; pos < nextp->size; ++pos,++p->size){
            p->data[p->size] = nextp->data[pos];
        }
        p->next = nextp->next;
        delete nextp;
    }
}



// Main function for debugging
int main() {
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

    return 0;
}