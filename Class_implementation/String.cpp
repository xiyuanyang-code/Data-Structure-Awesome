/*
This is the implementation of hand-made string
*/
#include <stdexcept>
#include <iostream>
#include <algorithm> //just for the min function
#include <cstring>


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


//main function is used for debugging only
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