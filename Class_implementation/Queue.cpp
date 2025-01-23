/*
 * @Author: Xiyuan Yang   xiyuan_yang@outlook.com
 * @Date: 2025-01-22 15:06:31
 * @LastEditors: Xiyuan Yang   xiyuan_yang@outlook.com
 * @LastEditTime: 2025-01-23 12:13:29
 * @FilePath: \CODE_for_Vscode\Data_structure\Class_implementation\Queue.cpp
 * @Description: 
 * Do you code and make progress today?
 * Copyright (c) 2025 by Xiyuan Yang, All Rights Reserved. 
 */
/*
This is the implementation of the stack
*/

#include <stdexcept>
#include <iostream>



//Abstract class
template <class T>
class queue{
    public:
        virtual ~queue(){}
        virtual bool isEmpty() const = 0;
        virtual T getHead() const = 0;
        virtual T pop()=0;
        virtual void push(const T& value)= 0;
};
//The abstract class has no data members, only providing the interface

//Sequential Implementation of stack
template <class T>
class seqQueue : public queue<T>{
    private:
        T* elem;//the elements
        int maxsize;
        int front,rear;
        void DoubleSpace();//Tool functions for space extensions
    public:
        seqQueue(int size = 10);
        // Todo: implement copy constructor and the overload of = (assignment)
        ~seqQueue();
        bool isEmpty() const;
        void push(const T& value);
        T getHead() const;
        T pop();

        //Only for debugging
        void print() const;
        int length() const;
};

template <class T>
seqQueue<T>::seqQueue(int size) : maxsize(size){
    elem = new T[maxsize];
    //but the maximum capacity is maxsize-1
    front = rear = 0;
}

template <class T>
seqQueue<T>::~seqQueue(){
    delete[] elem;
}

template <class T>
T seqQueue<T>::getHead() const{
    if (isEmpty()) {
        throw std::runtime_error("Queue is empty");
    }
    return elem[front];
}

template <class T>
bool seqQueue<T>::isEmpty() const{
    return front == rear;
}

template <class T>
void seqQueue<T>::push(const T& value){
    //Enter the queue, operate on the rear
    if((rear + 1) % maxsize == front){
        //judge if the queue is full
        DoubleSpace();
    }else{
        elem[rear] = value;
        rear = (rear + 1) % maxsize;
    }
}

template <class T>
T seqQueue<T>::pop(){
    if (isEmpty()) {
        throw std::runtime_error("Queue is empty");
    }
    //leave the queue, operate on the front
    T temp = elem[front];//the top element of the queue
    front = (front + 1)%maxsize;
    return temp;
}

template <class T>
void seqQueue<T>::DoubleSpace(){
    //double the space of the queue
    T* tmp = elem;
    elem = new T[2*maxsize];
    for(int i = 0; i < maxsize - 1; i++){
        elem[i] = tmp[(front + i) % maxsize];
        //you only need to copy maxsize-1 elements!
    }
    front = 0;
    rear = maxsize - 1;
    maxsize *= 2;
    delete[] tmp;
}

template <class T>
void seqQueue<T>::print()const {
    //print the whole queue
    if(isEmpty()){
        std::cout << "The queue is empty!" << std::endl;
        return;
    }else{
        std::cout << "The queue has " << length() << " members." << std::endl;
        int index = front;
        while(index != rear){
            std::cout << elem[index] << " ";
            index = (index + 1) % maxsize;
        }
        std::cout << std::endl;
    }
}

template <class T>
int seqQueue<T>::length() const{
    if(rear >= front) return rear - front;
    else return rear + maxsize - front;
}

//Linked implementation of queue (The definition)


template <class T>
class linkQueue : public queue<T>{
    private:
        struct Node
        {
            T data;
            Node* next;
            //parameterized constructor(default)
            Node(const T& value = 0, Node *p = nullptr){
                data = value;
                next = p;
            }
            //destructor
            ~Node(){}
        };
        Node *front, *rear;
        //two pointers point at the front or the end of the queue.
    public:
        linkQueue();
        //This constructor is used to create a new linked queue.
        //Todo: implement copy constructor and the overload of = (assignment)
        ~linkQueue();
        bool isEmpty() const;
        T pop();
        T getHead() const;
        void push(const T& value);

        //for debugging
        void print() const;
        int length() const;
        
};


//Implementation of linked queue
template <class T>
linkQueue<T>::linkQueue(){
    //set nullptr for both pointers
    front = rear = nullptr;
    //it is also the judgement of an empty queue
}

template <class T>
linkQueue<T>::~linkQueue(){
    Node * temp;
    while(front != nullptr){
        //simulate the process of leaving the queue until the queue is empty.
        temp = front;
        front = front -> next;
        delete temp;
    }
}

template <class T>
bool linkQueue<T>::isEmpty() const {
    return front == nullptr;
}

template <class T>
T linkQueue<T>::getHead() const{
    if (isEmpty()) {
        throw std::runtime_error("Queue is empty");
    }
    return front -> data;
}

template <class T>
void linkQueue<T>::push(const T& value){
    if(isEmpty()){
        front = rear = new Node(value);
    }else{
        Node *newnode = new Node(value);
        rear -> next = newnode;
        rear = rear -> next;
        //! ensure the rear -> next often points to the nullptr
    }
}

template <class T>
T linkQueue<T>::pop(){
    if (isEmpty()) {
        throw std::runtime_error("Queue is empty");
    }else{
        T ans = front -> data;
        Node* tmp = front;
        front = front -> next;
        if(front == nullptr){
            rear = nullptr;
            //now the queue is empty!
            //This is necessary, because before the pop process, the front and the rear pointer points at the same space
            //now front moves to nullptr, rear pointer becomes INVALID! 
        }
        delete tmp;
        return ans;
    }
}

template <class T>
int linkQueue<T>::length() const {
    if(isEmpty()){
        return 0;
    }
    int length=0;
    Node* traverseptr = front;
    while(traverseptr != nullptr){
        traverseptr = traverseptr -> next;
        length++; 
    }
    return length;
}

template <class T>
void linkQueue<T>::print()const {
    if(isEmpty()){
        std::cout << "The queue is empty!" << std::endl;
        return;
    }else{
        int length=0;
        Node* traverseptr = front;
        while(traverseptr != nullptr){
            std::cout << traverseptr -> data << " ";
            traverseptr = traverseptr -> next;
            length++; 
        }
        std::cout << std::endl;
        std::cout << "The queue has " << length << " elements" << std::endl;
    }
}

//The main function is used for Debugging only.
int main(){
    seqQueue<int> list;
    list.print();
    list.push(2);
    list.push(5);
    std::cout << list.isEmpty() << std::endl;
    list.print();
    std::cout << list.pop();
    std::cout << list.pop();
    std::cout << list.isEmpty() << std::endl;
    //The debug of link queue
    std::cout << std::endl;
    linkQueue<int> list2;
    list2.print();
    for(int i = 0; i < 5; i++){
        list2.push(i * i);
        list2.print();
    }

    std::cout << list2.isEmpty() << std::endl;
    for (int i = 0; i < 5; i++)
    {
        std::cout << list2.pop() <<std::endl;
        list2.print();
    }
    
    std::cout << list2.isEmpty() << std::endl;
    return 0;
}