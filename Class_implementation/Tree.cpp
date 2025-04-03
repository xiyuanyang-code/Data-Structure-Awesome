/*
 * @Author: Xiyuan Yang   xiyuan_yang@outlook.com
 * @Date: 2025-02-19 20:13:27
 * @LastEditors: Xiyuan Yang   xiyuan_yang@outlook.com
 * @LastEditTime: 2025-03-26 21:21:13
 * @FilePath: /Data_structure/single_files/Class_implementation/Tree.cpp
 * @Description: 
 * Do you code and make progress today?
 * Copyright (c) 2025 by Xiyuan Yang, All Rights Reserved. 
 */
/*
The implementation of Tree and Binary Tree
*/
#include <iostream>
#include <stack>
#include <queue>
//Abstract class 
template <class T>
class Tree{
    public:
        virtual void create() = 0;
        virtual bool isEmpty() const = 0;
        virtual T root(T invalid_flag) const = 0;
        virtual T parent(T current_node, T invalid_flag) const = 0;
        virtual T child(T current_node, int tree_index, T invalid_flag) const = 0;
        virtual void remove (T current_node, int tree_index) = 0;
        virtual void traverse () const = 0;

};

//Linked List implementation of Binary Tree
template <class T>
class bTree{
    public:
        virtual void clear() = 0;
        virtual bool isEmpty() = 0;
        virtual T Root(T flag) = 0;
        virtual T parent(T x, T flag)const = 0;
        virtual T lchild(T x, T flag)const = 0;
        virtual T rchild(T x, T flag)const = 0;
        virtual void delLeft(T x) = 0;
        virtual void delRight(T x) = 0;
        virtual void preOrder() const = 0;
        virtual void midOrder() const = 0;
        virtual void postOrder() const = 0;
        virtual void levelOrder() const = 0;
};




//二叉链表类
template <class T>
class binaryTree: public bTree<T> {
    template <class U>
    friend void printTree (const binaryTree<U> &t,U flag);
private:
    struct Node {
        Node *left,*right;
        T data;

        Node(): left(nullptr),right(nullptr){}
        Node(T item,Node *L=nullptr,Node *R=nullptr): data(item),left(L),right(R){}
        ~Node(){}
    };

    Node *root;     //二叉树的根节点
public:
    binaryTree(): root(nullptr){}
    binaryTree(T x){root=new Node(x);}
    ~binaryTree(){clear(root);}
    void clear(){clear(root);}
    bool isEmpty() const{return root == nullptr;}
    T Root(T flag) const{if(root == 0)return flag;else return root->data;}
    T parent(T x,T flag) const{return flag;}
    T l_child(T x,T flag) const;
    T r_child(T x,T flag) const;
    void delRight(T x) ;
    void delLeft(T x) ;
    void preOrder() const;
    void midOrder() const;
    void postOrder() const;
    void levelOrder() const;
    void creatTree(T flag);
    int size()const{return size(root);}
    int height()const{return height(root);}
private:
    Node *find(T x,Node *t) const;
    void clear(Node *&t);
    void preOrder(Node *t) const;
    void midOrder(Node *t) const;
    void postOrder(Node *t) const;
    int size(binaryTree<T>::Node *t)const;
    int height(binaryTree<T>::Node *t)const;
};

//清空
template<class T>
void binaryTree<T>::clear(binaryTree<T>::Node *&t) {        //使用引用的指针，对指针进行修改
    if(t == nullptr)return;
    clear(t->left);
    clear(t->right);
    delete t;
    t=nullptr;
}

//递归实现的遍历操作
//前序
template<class T>
void binaryTree<T>::preOrder(Node *t) const {
    if(t == nullptr)return;
    std::cout << t->data <<' ';
    preOrder(t->left);
    preOrder(t->right);
}
// template<class T>
// void binaryTree<T>::preOrder() const {
//     cout << " 前序遍历：";
//     preOrder(root);
// }

//后序
template<class T>
void binaryTree<T>::postOrder(Node *t) const {
    if(t == nullptr)return;
    postOrder(t->left);
    postOrder(t->right);
    std::cout << t->data <<' ';
}
// template<class T>
// void binaryTree<T>::postOrder() const {
//     cout << " 后序遍历：";
//     postOrder(root);
// }

//中序
template<class T>
void binaryTree<T>::midOrder(Node *t) const {
    if(t == nullptr)return;
    midOrder(t->left);
    std::cout << t->data <<' ';
    midOrder(t->right);
}
// template<class T>
// void binaryTree<T>::midOrder() const {
//     cout << " 中序遍历：";
//     midOrder(root);
// }
// 层次序遍历实现

//层次序
template <class T>
void binaryTree<T>::levelOrder() const {
    if (root == nullptr) return;

    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();
        std::cout << current->data << " ";

        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
    std::cout << std::endl;
}

//递归操作的非遍历实现
// 非递归前序遍历
template <class T>
void binaryTree<T>::preOrder() const {
    if (root == nullptr) return;

    std::stack<Node*> s;
    s.push(root);

    while (!s.empty()) {
        Node* current = s.top();
        s.pop();
        std::cout << current->data << " ";

        if (current->right) s.push(current->right);
        if (current->left) s.push(current->left);
    }
    std::cout << std::endl;
}

// 非递归中序遍历
template <class T>
void binaryTree<T>::midOrder() const {
    std::stack<Node*> s;
    Node* current = root;

    while (current != nullptr || !s.empty()) {
        while (current != nullptr) {
            s.push(current);
            current = current->left;
        }

        current = s.top();
        s.pop();
        std::cout << current->data << " ";
        current = current->right;
    }
    std::cout << std::endl;
}

// 非递归后序遍历
template <class T>
void binaryTree<T>::postOrder() const {
    if (root == nullptr) return;

    std::stack<Node*> s1, s2;
    s1.push(root);

    while (!s1.empty()) {
        Node* current = s1.top();
        s1.pop();
        s2.push(current);

        if (current->left) s1.push(current->left);
        if (current->right) s1.push(current->right);
    }

    while (!s2.empty()) {
        std::cout << s2.top()->data << " ";
        s2.pop();
    }
    std::cout << std::endl;
}

//私有，以某节点为根的树的大小
template <class T>
int binaryTree<T>::size(binaryTree<T>::Node *t)const {
    if(t == nullptr) return 0;
    return 1 + size(t->left) + size(t->right);
}

//私有，以某节点为根的树的高度
template <class T>
int binaryTree<T>::height(binaryTree<T>::Node *t)const {
    if(t == nullptr) return 0;
    else {
        int lt=height(t->left),rt=height(t->right);
        return 1+((lt>rt)?lt:rt);
    }
}

//私有，查找以t为根的树中值为x的结点，并返回指针
template<class T>
binaryTree<T>::Node* binaryTree<T>::find(T x, binaryTree<T>::Node *t) const {
    Node *tmp;
    if(t == nullptr){
        return nullptr;
    }
    
    if(t->data == x){
        return t;
    }

    if(tmp = find(x,t->left)){
        return tmp;
    }else{
        return find(x,t->right);
    }
}

//删除左子树
template<class T>
void binaryTree<T>::delLeft(T x) {
    Node *tmp=find(x,root);
    if(tmp == nullptr)return ;
    clear (tmp->left);
}

//删除右子树
template<class T>
void binaryTree<T>::delRight(T x) {
    Node *tmp=find(x,root);
    if(tmp == nullptr)return ;
    clear (tmp->right);
}

//查找值为x的结点的右子节点值
template<class T>
T binaryTree<T>::r_child(T x, T flag) const {
    Node *tmp=find(x,root);
    if(tmp ==nullptr || tmp->right == nullptr)return flag;
    return tmp->right->data;
}

//查找值为x的结点的左子节点值
template<class T>
T binaryTree<T>::l_child(T x, T flag) const {
    Node *tmp=find(x,root);
    if(tmp ==nullptr || tmp->left == nullptr)return flag;
    return tmp->left->data;
}

//创建树
template<class T>
void binaryTree<T>::creatTree(T flag) {
    std::queue<Node *>que;
    Node *tmp;
    T x,l_data,r_data;
    //创建树，输入flag表示空
    std::cout << "Enter the root node";
    std::cin >> x;
    root = new Node(x);
    que.push(root);
    while(!que.empty()) {
        tmp = que.top();
        que.pop();
        std::cout << "Enter" << tmp->data <<"two sons (" << flag << ") represents the null node";
        std::cin >> l_data >> r_data;
        if(l_data != flag){
            que.push(tmp -> left = new Node(l_data));
        }
        if(r_data != flag){
            que.push(tmp->right = new Node(r_data));
        }
    }
}

//输出树
template<class T>
void printTree(const binaryTree<T>&t,T flag) {
    std::queue<T>q;
    q.enQueue(t.root->data);
    std::cout << std::endl;
    while(!q.isEmpty()) {
        char p,l,r;
        p=q.deQueue();
        l=t.l_child(p,flag);
        r=t.r_child(p,flag);
        std::cout << p << " " << l << " " << r << std::endl;
        if(l != flag)q.enQueue(l);
        if(r != flag)q.enQueue(r);
    }
}

