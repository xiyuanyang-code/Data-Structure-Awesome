/*
The implementation of Tree and Binary Tree
*/

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