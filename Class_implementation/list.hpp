#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "Exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cmath>
#include <cstddef>
#include <cstring>
#include <memory>


namespace sjtu {
    /**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
    template<typename T>
    class list {
    protected:
        class node {
        public:
            T *val;
            node *prev;
            node *next;

            bool is_dummy;

            /**
         * @brief constructor and destructor
         * 
         */
            // for none-dummy nodes, we use allocate to construct a new object
            node(const T &value, node *prev_ = nullptr, node *next_ = nullptr) : prev(prev_), next(next_) {
                is_dummy = false;

                std::allocator<T> alloc;
                val = alloc.allocate(1);
                std::allocator_traits<std::allocator<T>>::construct(alloc, val, value);
            }

            node(const node &other) : prev(nullptr), next(nullptr), is_dummy(other.is_dummy) {
                std::allocator<T> alloc;
                if (is_dummy == false) {
                    val = alloc.allocate(1);
                    std::allocator_traits<std::allocator<T>>::construct(alloc, val, *(other.val));
                }
            }

            // for dummy nodes, we use default constructor
            node(node *prev_ = nullptr, node *next_ = nullptr) : prev(prev_), next(next_) {
                is_dummy = true;
                val = nullptr;
            }

            ~node() {
                // only deallocate for the dummy node
                if (is_dummy == false) {
                    std::allocator<T> alloc;
                    std::allocator_traits<std::allocator<T>>::destroy(alloc, val);
                    alloc.deallocate(val, 1);
                }
            }
        };

    protected:
        node *head;
        node *tail;
        // the head and tail pointer are both dummy nodes

        size_t current_length;//current length of the linked list


        /**
     * insert node cur before node pos
     * return the inserted node cur
     */
        node *insert(node *pos, node *cur) {
            cur->prev = pos->prev;
            cur->next = pos;

            (cur->prev)->next = cur;
            pos->prev = cur;
            ++current_length;
            return cur;
        }

        /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
        node *erase(node *pos) {
            (pos->prev)->next = pos->next;
            (pos->next)->prev = pos->prev;
            --current_length;

            // the pos is removed, but not deleted
            return pos;
        }

    public:
        class const_iterator;
        class iterator {
            friend class const_iterator;
            friend class list<T>;

        private:
            node *Node;
            list *whichlist;
            //the head is only for specifying which list it points

        public:
            //constructor
            iterator(node *Node_ = nullptr, list *whichlist = nullptr) : Node(Node_), whichlist(whichlist) {}
            iterator(const iterator &other) : Node(other.Node), whichlist(other.whichlist) {}

            // return the private data members
            node *&getnode() {
                return Node;
            }


            list *&getlist() {
                return whichlist;
            }

            /**
         * iter++
         */
            iterator operator++(int) {
                if (Node == whichlist->tail) {
                    throw sjtu::invalid_iterator();
                }
                iterator tmp = *this;
                Node = Node->next;
                return tmp;
            }

            /**
         * ++iter
         */
            iterator &operator++() {
                if (Node == whichlist->tail) {
                    throw sjtu::invalid_iterator();
                }
                Node = Node->next;
                return *this;
            }

            /**
         * iter--
         */
            iterator operator--(int) {
                if (Node == whichlist->head->next) {
                    // the first element
                    throw sjtu::invalid_iterator();
                }
                iterator tmp = *this;
                Node = Node->prev;
                return tmp;
            }

            /**
         * --iter
         */
            iterator &operator--() {
                if (Node == whichlist->head->next) {
                    throw sjtu::invalid_iterator();
                }
                Node = Node->prev;
                return *this;
            }

            /**
         * @brief *it
         * remember to throw if iterator is invalid
         */
            T &operator*() const {
                if (Node == nullptr) {
                    //this is end() iterator
                    throw sjtu::invalid_iterator();
                }

                if (whichlist == nullptr) {
                    throw sjtu::invalid_iterator();
                }

                if (Node == whichlist->head || Node == whichlist->tail) {
                    // dummy nodes don't store data
                    throw sjtu::invalid_iterator();
                }

                return *(Node->val);
            }

            /**
         * @brief it->field
         * ? remember to throw if iterator is invalid
         */
            T *operator->() const noexcept {
                return (Node->val);
            }

            /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
            bool operator==(const iterator &rhs) const {
                return (Node == (rhs.Node));
            }
            bool operator==(const const_iterator &rhs) const {
                return (Node == (rhs.Node));
            }

            /**
         * some other operator for iterator.
         */
            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }
            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }
        };

        /**
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
        class const_iterator {
            friend class iterator;

        private:
            const node *Node;
            const list *whichlist;

        public:
            const_iterator(const node *Node_ = nullptr, const list *whichlist = nullptr) : Node(Node_), whichlist(whichlist) {}
            const_iterator(const iterator &other) {
                Node = other.Node;
                whichlist = other.whichlist;
            }
            ~const_iterator() = default;

            const node *getnode() const {
                return Node;
            }
            const list *getlist() const {
                return whichlist;
            }

            /**
         * iter++
         */
            const_iterator operator++(int) {
                if (Node == whichlist->tail) {
                    throw sjtu::invalid_iterator();
                }
                const_iterator tmp = *this;
                Node = Node->next;
                return tmp;
            }

            /**
         * ++iter
         */
            const_iterator &operator++() {
                if (Node == whichlist->tail) {
                    throw sjtu::invalid_iterator();
                }
                Node = Node->next;
                return *this;
            }

            /**
         * iter--
         */
            const_iterator operator--(int) {
                if (Node == whichlist->head->next) {
                    throw sjtu::invalid_iterator();
                }
                const_iterator tmp = *this;
                Node = Node->prev;
                return tmp;
            }

            /**
         * --iter
         */
            const_iterator &operator--() {
                if (Node == whichlist->head->next) {
                    throw sjtu::invalid_iterator();
                }
                Node = Node->prev;
                return *this;
            }

            /**
         * @brief *it
         * remember to throw if iterator is invalid
         */
            const T &operator*() const {
                if (Node == nullptr) {
                    //this is end() iterator
                    throw sjtu::invalid_iterator();
                }

                if (whichlist == nullptr) {
                    throw sjtu::invalid_iterator();
                }

                if (Node == whichlist->head || Node == whichlist->tail) {
                    // dummy nodes don't store data
                    throw sjtu::invalid_iterator();
                }

                return *(Node->val);
            }

            /**
         * @brief it->field
         * remember to throw if iterator is invalid
         */
            const T *operator->() const noexcept {
                return (Node->val);
            }

            /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
            bool operator==(const const_iterator &rhs) const {
                return (Node == (rhs.Node));
            }
            bool operator==(const iterator &rhs) const {
                return (Node == (rhs.Node));
            }

            /**
         * some other operator for iterator.
         */
            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }
            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }
        };


        /**
     * @brief Construct a new list object, creating a new linked list
     */
        list() {
            // construct two dummy node using the default constructor
            head = new node();
            tail = new node(head);
            current_length = 0;
            head->next = tail;
            tail->prev = head;
        }

        /**
     * @brief Construct a new list object using the copy constructor
     * 
     * @param other 
     */
        list(const list &other) {
            // first construct an empty list
            head = new node();
            tail = new node(head);
            current_length = 0;
            head->next = tail;
            tail->prev = head;

            // use push back to add elements
            for (auto it = other.head->next; it != other.tail; it = it->next) {
                push_back(*(it->val));
            }
        }


        /**
     * @brief Destroy the list object
     * 
     */
        virtual ~list() {
            //clear all nodes except for the dummy node in thr tail
            clear();

            //destory the dummy node;
            delete head;
            delete tail;
        }


        /**
     * @brief assignment operator
     * 
     * @param other 
     * @return list& 
     */
        list &operator=(const list &other) {
            if (&other == this) {
                return *this;
            }

            clear();//reset to the empty node

            for (node *it = other.head->next; it != other.tail; it = it->next) {
                push_back(*(it->val));
            }
            return *this;
        }

        /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
        const T &front() const {
            if (empty()) {
                throw sjtu::container_is_empty();
            } else {
                return *(head->next->val);
            }
        }

        const T &back() const {
            if (empty()) {
                throw sjtu::container_is_empty();
            } else {
                return *((tail->prev)->val);
            }
        }

        /**
     * returns an iterator to the beginning.
     * ! the head pointer will never become an iterator!!!
     */
        iterator begin() {
            return iterator(head->next, this);
            // if the list is empty, then the begin() equals to the end() pointer
        }
        const_iterator cbegin() const {
            return const_iterator(head->next, this);
        }

        /**
     * returns an iterator to the end.
     */
        iterator end() {
            return iterator(tail, this);
        }
        const_iterator cend() const {
            return const_iterator(tail, this);
        }


        /**
     * checks whether the container is empty.
     */
        virtual bool empty() const {
            return current_length == 0;
        }
        /**
     * returns the number of elements
     */
        virtual size_t size() const {
            return current_length;
        }

        /**
     * clears the contents
     */
        virtual void clear() {
            node *it = head->next;
            while (it != tail) {
                node *nextptr = it->next;
                delete it;
                it = nextptr;
            }

            // reset the data members
            head->next = tail;
            tail->prev = head;
            current_length = 0;
        }

        /**
     * insert value before pos_node (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
        iterator insert(iterator pos_it, const T &value) {
            if (pos_it.getlist() != this) {
                throw sjtu::invalid_iterator();
            }

            if (pos_it.getnode() == nullptr || pos_it.getnode() == head) {
                throw sjtu::invalid_iterator();
            }


            node *new_node = new node(value);
            insert(pos_it.Node, new_node);
            return iterator(new_node, this);
        }

        /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
        virtual iterator erase(iterator pos) {
            if (pos.getlist() != this) {
                throw sjtu::invalid_iterator();
            }

            if (pos.getnode() == head || pos.getnode() == tail || pos.getnode() == nullptr) {
                throw sjtu::invalid_iterator();
            }

            if (empty()) {
                throw sjtu::container_is_empty();
            }

            node *next_node = pos.Node->next;

            erase(pos.Node);
            delete pos.Node;
            return iterator(next_node, this);
        }

        /**
     * adds an element to the end
     */
        void push_back(const T &value) {
            insert(end(), value);
        }

        /**
     * removes the last element
     * throw when the container is empty.
     */
        void pop_back() {
            if (empty()) {
                throw sjtu::container_is_empty();
            } else {
                auto it = end();
                it--;
                erase(it);
            }
        }


        /**
     * inserts an element to the beginning.
     */
        void push_front(const T &value) {
            insert(begin(), value);
        }

        /**
     * removes the first element.
     * throw when the container is empty.
     */
        void pop_front() {
            if (empty()) {
                throw sjtu::container_is_empty();
            } else {
                erase(begin());
            }
        }


        /**
     * sort the values in ascending order with operator< of T
     */
        void sort() {
            if (current_length <= 1) return;

            //sorting using the merge
            node **nodesarr = new node *[current_length];
            node *current = head->next;
            for (size_t i = 0; i < current_length; ++i) {
                nodesarr[i] = current;
                current = current->next;
                //don't care about the tail node
            }


            sjtu::sort<node *>(nodesarr, nodesarr + current_length, std::function<bool(const node *, const node *)>([](const node *a, const node *b) -> bool {
                                   return *(a->val) < *(b->val);
                               }));

            //connect the list!

            head->next = nodesarr[0];
            nodesarr[0]->prev = head;
            for (size_t i = 0; i < current_length - 1; ++i) {
                nodesarr[i]->next = nodesarr[i + 1];
                nodesarr[i + 1]->prev = nodesarr[i];
            }

            nodesarr[current_length - 1]->next = tail;
            tail->prev = nodesarr[current_length - 1];

            //release the array
            for (size_t i = 0; i < current_length; ++i) {
                std::destroy_at(nodesarr + i);
            }
            delete[] nodesarr;
        }


        /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
        void merge(list &other) {
            if (this == &other) return;
            if (other.empty()) return;

            node *it_this = head->next;
            node *it_other = other.head->next;

            // connect the two list
            while (it_this != tail && it_other != other.tail) {
                // if this > other
                if (*(it_other->val) < *(it_this->val)) {
                    // insert the element after the node
                    node *tmp = it_other->next;
                    other.erase(it_other);
                    insert(it_this, it_other);
                    it_other = tmp;
                } else {
                    // just do the update process
                    it_this = it_this->next;
                }
            }

            // clean the remaining
            while (it_other != other.tail) {
                node *tmp = it_other->next;
                other.erase(it_other);
                insert(it_this, it_other);
                it_other = tmp;
            }
        }

        /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
        void reverse() {
            if (current_length <= 1) return;

            node *current = head->next;

            // reverse the next and prev for every node!
            while (current && current != tail) {
                std::swap(current->prev, current->next);
                current = current->prev;
            }

            // for the head and the tail
            std::swap(head->next, tail->prev);

            head->next->prev = head;
            tail->prev->next = tail;
        }

        /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
        void unique() {
            if (current_length <= 1) return;

            node *current = head->next;
            while (current != tail && current->next != tail) {
                //while current is the last element of x, jump out of the loop
                if (*(current->val) == *(current->next->val)) {

                    //delete the node
                    node *duplicate = current->next;
                    erase(duplicate);
                    delete duplicate;
                } else {
                    current = current->next;
                }
            }
        }
    };

}// namespace sjtu

#endif//SJTU_LIST_HPP
