/*
 * @Author: Xiyuan Yang   xiyuan_yang@outlook.com
 * @Date: 2025-01-22 15:06:31
 * @LastEditors: Xiyuan Yang   xiyuan_yang@outlook.com
 * @LastEditTime: 2025-01-22 23:49:35
 * @FilePath: \CODE_for_Vscode\Data_structure\Class_implementation\Queue.cpp
 * @Description: 
 * Do you code and make progress today?
 * Copyright (c) 2025 by Xiyuan Yang, All Rights Reserved. 
 */
/*
This is the implementation of the stack
*/

#pragma once
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
//抽象基类仅仅定义接口，没有具体的数据成员

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
    //入队操作，需要对rear操作
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
    //出队操作，对front操作
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
    return 0;
}