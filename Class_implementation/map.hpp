/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP
// only for std::less<T>
#include "Exceptions.hpp"
#include "utility.hpp"
#include <cmath>
#include <cstddef>
#include <functional>
namespace sjtu {
    template<class Key, class T, class Compare = std::less<Key>>
    class map {
    public:
        class iterator;
        class const_iterator;
        typedef pair<const Key, T> value_type;
        friend class iterator;
        friend class const_iterator;

    private:
        struct Node {
            value_type *data;
            Node *left, *right;
            Node *prev, *next;
            // the height of the node
            size_t height;
            // for no-dummy node
            Node(const value_type &element, Node *left_ = nullptr, Node *right_ = nullptr, Node *prev_ = nullptr, Node *next_ = nullptr, size_t height_ = 1)
                : right(right_), left(left_), prev(prev_), next(next_), height(height_) {
                data = new value_type(element);
            }
            // for dummy node, default constructor
            Node() {
                data = nullptr;
                left = right = prev = next = nullptr;
                height = 1;
            }
            ~Node() {
                if (data != nullptr) {
                    delete data;
                }
            }
        };
        // private data members
        Node *root;
        // the head and tail are dummy pointers!
        Node *head;
        Node *tail;
        size_t current_size;

    private:
        // several private functions
        /**
         * @brief recursion version of copying an AVL Tree
         *
         * @param root_
         * @return Node*
         */
        Node *copy(Node *root_) {
            if (root_ == nullptr) {
                return nullptr;
            }
            Node *left_new = (root_->left != nullptr) ? copy(root_->left) : nullptr;
            Node *right_new = (root_->right != nullptr) ? copy(root_->right) : nullptr;
            // copy the current root node
            Node *current = new Node(*(root_->data), left_new, right_new, nullptr, nullptr, root_->height);
            return current;
        }

        /**
         * @brief make the linked list of the Tree
         *
         * @param root_
         * @param head_
         * @return Node*: return the last node of of the subtree in the traversal order
         */
        Node *link_order(Node *root_, Node *head_) {
            if (root_ == nullptr) {
                return nullptr;
            }
            // traverse the root node and link them in a linked list
            // traverse the left subtree
            Node *left_sub_node = root_->left;
            Node *last_in_left_sub = link_order(left_sub_node, head_);
            if (last_in_left_sub != nullptr) {
                // then the linked list should be: last_in_left_sub -> root_
                last_in_left_sub->next->prev = root_;
                root_->next = last_in_left_sub->next;
                last_in_left_sub->next = root_;
                root_->prev = last_in_left_sub;
            } else {
                // the left subtree is empty
                // deal with the head (dummy) pointer
                head_->next->prev = root_;
                root_->next = head_->next;
                head_->next = root_;
                root_->prev = head_;
            }
            if (root_->right == nullptr) {
                // root is the last element!
                return root_;
            } else {
                // traverse the right subtree
                // !root_ is the new head of the sub linked list
                return link_order(root_->right, root_);
            }
        }

        /**
         * @brief recursive version of clear
         *
         * @param current
         */
        void clearNode(Node *current) {
            if (current == nullptr) {
                return;
            }
            // clear left and right subtrees
            clearNode(current->left);
            clearNode(current->right);
            // delete the current node
            delete current;
            // reset the root node
            current = nullptr;
        }

        Node *find_val(Node *t, const Key &val) const {
            Node *cur = t;
            while (cur != nullptr) {
                if (Compare()(val, cur->data->first)) {
                    cur = cur->left;
                } else if (Compare()(cur->data->first, val)) {
                    cur = cur->right;
                } else {
                    break;// found
                }
            }
            return cur;
        }

        size_t height(Node *t) const {
            if (t == nullptr) {
                return 0;
            } else {
                return t->height;
            }
        }

        void updateHeight(Node *&root_) {
            if (root_ == nullptr) {
                return;
            } else {
                root_->height = std::max(height(root_->left), height(root_->right)) + 1;
            }
        }

        // rotations operations for AVL Tree
        void LL(Node *&danger) {
            Node *t1 = danger->left;
            // rotate to adjust the structure
            danger->left = t1->right;
            t1->right = danger;
            // update height
            updateHeight(danger);
            updateHeight(t1);
            danger = t1;
        }

        /**
         * @brief Rotation if the insertion happens in the right child of the right child of the danger node
         *
         * @param danger The danger node
         */
        void RR(Node *&danger) {
            // The same with LL rotations
            Node *t1 = danger->right;
            // rotate RR
            danger->right = t1->left;
            t1->left = danger;
            // update height
            updateHeight(danger);
            updateHeight(t1);
            danger = t1;
        }

        /**
         * @brief danger.leftchild.rightchild = insertion_place
         *
         * @param danger
         */
        void LR(Node *&danger) {
            RR(danger->left);
            LL(danger);
        }

        /**
         * @brief danger.rightchild.leftchild = insertion_place
         *
         * @param root_
         */
        void RL(Node *&danger) {
            LL(danger->right);
            RR(danger);
        }

        /**
         * @brief Judge whether the tree remain the same height after the deletion
         *
         * @param root_ the current root node
         * @param subTree 1 for deleting on the right child, 0 for left child
         * @return true
         * @return false
         */
        bool adjust(Node *&root_, int subTree) {
            if (subTree) {
                // Delete on the right child
                if (height(root_->left) - height(root_->right) == 1) {
                    // Case1: the height remains unchanged
                    return true;
                } else if (height(root_->left) == height(root_->right)) {
                    --(root_->height);
                    return false;
                } else if (height((root_->left)->right) > height((root_->left)->left)) {
                    LR(root_);
                    // The height changed!
                    return false;
                } else {
                    LL(root_);
                    if (height(root_->right) == height(root_->left)) {
                        return false;
                    } else {
                        return true;
                    }
                }
            } else {
                if (height(root_->right) - height(root_->left) == 1) {
                    // Case1: the height remains unchanged
                    return true;
                } else if (height(root_->left) == height(root_->right)) {
                    --(root_->height);
                    return false;
                } else if (height((root_->right)->left) > height((root_->right)->right)) {
                    RL(root_);
                    // The height changed!
                    return false;
                } else {
                    RR(root_);
                    if (height(root_->right) == height(root_->left)) {
                        return false;
                    } else {
                        return true;
                    }
                }
            }
        }

        /**
         * @brief insert_node: insert a value in the subtree of root_
         *
         * @param value The value to insert
         * @param root_ The current root of the subtree
         * @return true If the insertion was successful
         * @return false If the key already exists and insertion failed
         */
        bool insert_node(const value_type &value, Node *&root_) {
            bool success = true;
            if (root_ == nullptr) {
                // we find the position!
                current_size++;
                // find the last node
                Node *previous = nullptr;
                Node *current = root;
                while (current != nullptr) {
                    if (Compare()(current->data->first, value.first) && (previous == nullptr || Compare()(previous->data->first, current->data->first))) {
                        previous = current;
                    }
                    if (Compare()(value.first, current->data->first)) {
                        current = current->left;
                    } else {
                        current = current->right;
                    }
                }
                if (previous != nullptr) {
                    // default height is 1
                    root_ = new Node(value, nullptr, nullptr, previous, previous->next);
                } else {
                    root_ = new Node(value, nullptr, nullptr, head, head->next);
                }
                // adjust the linked list
                root_->prev->next = root_;
                root_->next->prev = root_;
            } else if (Compare()(value.first, root_->data->first)) {
                // insert on the left subtree
                if (insert_node(value, root_->left) == false) {
                    success = false;
                }
                // rotate the tree
                if ((height(root_->left) - height(root_->right)) == 2) {
                    if (Compare()(value.first, root_->left->data->first)) {
                        // rotate LL
                        LL(root_);
                    } else {
                        LR(root_);
                    }
                }
            } else if (Compare()(root_->data->first, value.first)) {
                // insert on the right subtree
                if (insert_node(value, root_->right) == false) {
                    success = false;
                }
                // rotate the tree
                if ((height(root_->left) - height(root_->right)) == -2) {
                    if (Compare()(root_->right->data->first, value.first)) {
                        // rotate LL
                        RR(root_);
                    } else {
                        RL(root_);
                    }
                }
            } else {
                // we have found the key previously!
                success = false;
            }
            // update the height of root_
            updateHeight(root_);
            return success;
        }

        /**
         * @brief delete a node from the subtree of root_ with the key value of key
         *
         * @param key The key of the node to remove
         * @param root_ The current root of the subtree
         * @return true If the height of the subtree remained unchanged
         * @return false If the height of the subtree has changed
         */
        bool remove_node(const Key &key, Node *&root_) {
            // if we doesn't find the value
            if (root_ == nullptr) {
                return true;
            }
            // if find the exact value
            if (!(Compare()(key, root_->data->first) || Compare()(root_->data->first, key))) {
                if (root_->left == nullptr || root_->right == nullptr) {
                    current_size--;
                    Node *current = root_;
                    // fix the linked list
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                    if (root_->left != nullptr) {
                        root_ = root_->left;
                    } else {
                        root_ = root_->right;
                    }
                    delete current;
                    return false;
                } else {
                    Node *successor = root_->right, *successorParent = root_->right;
                    while (successor->left != nullptr) {
                        successorParent = successor;
                        successor = successor->left;
                    }
                    Node *parent = root, *target = root_;
                    while (target != root && parent->left != target && parent->right != target)
                        parent = (Compare()(key, parent->data->first) ? parent->left : parent->right);
                    if (successor == target->right) {
                        Node *leftChild = target->left;
                        int oldHeight = target->height;
                        target->left = successor->left;
                        target->right = successor->right;
                        target->height = successor->height;
                        successor->left = leftChild;
                        successor->right = target;
                        successor->height = oldHeight;
                        if (target != root) {
                            if (parent->left == target) parent->left = successor;
                            else
                                parent->right = successor;
                        } else
                            root = successor;
                    } else {
                        Node *leftChild = target->left, *rightChild = target->right;
                        int oldHeight = target->height;
                        target->left = successor->left;
                        target->right = successor->right;
                        target->height = successor->height;
                        successor->left = leftChild;
                        successor->right = rightChild;
                        successorParent->left = target;
                        if (target != root) {
                            if (parent->left == target) parent->left = successor;
                            else
                                parent->right = successor;
                        } else
                            root = successor;
                        successor->height = oldHeight;
                    }
                    delete target->data;
                    target->data = new value_type(*(successor->data));
                    if (remove_node(target->data->first, root_->right)) return true;
                    return adjust(root_, 1);
                }
            }
            if (Compare()(key, root_->data->first)) {
                if (remove_node(key, root_->left)) {
                    return true;
                } else {
                    return adjust(root_, 0);
                }
            } else {
                if (remove_node(key, root_->right)) {
                    return true;
                } else {
                    return adjust(root_, 1);
                }
            }
        }

        /**
         * @brief find the parent node of the given node
         *
         * @param root
         * @param child
         * @return Node*
         */
        Node *find_parent(Node *root, Node *child) {
            if (root == nullptr || root == child) {
                return nullptr;
            }
            if (root->left == child || root->right == child) {
                return root;
            }
            if (Compare()(child->data->first, root->data->first)) {
                return find_parent(root->left, child);
            } else {
                return find_parent(root->right, child);
            }
        }

    public:
        class iterator {
            friend class const_iterator;
            friend class map;

        private:
            Node *pos;
            map *which_map;

        public:
            /**
             * @brief Construct a new iterator object
             *
             * @param pos_
             * @param which_map_
             */
            iterator(Node *pos_ = nullptr, map *which_map_ = nullptr) : pos(pos_), which_map(which_map_) {}

            /**
             * @brief Construct a new iterator object
             *
             * @param other
             */
            iterator(const iterator &other) : pos(other.pos), which_map(other.which_map) {}

            /**
             * @brief iter++ operation (traversal order)
             *
             * @return iterator
             */
            iterator operator++(int) {
                iterator backup = *this;
                // exception: if pos is the last element in the traversal order
                if (pos == which_map->tail) {
                    throw sjtu::invalid_iterator();
                }
                pos = pos->next;
                return backup;
            }

            /**
             * @brief ++iter operation (traversal order)
             *
             * @return iterator&
             */
            iterator &operator++() {
                if (pos == which_map->tail) {
                    throw sjtu::invalid_iterator();
                }
                pos = pos->next;
                return *this;
            }

            /**
             * @brief iter--
             *
             * @return iterator
             */
            iterator operator--(int) {
                // exception: if it is the begin, throw
                iterator backup = *this;
                if (pos->prev == which_map->head) {
                    throw sjtu::invalid_iterator();
                }
                pos = pos->prev;
                return backup;
            }

            /**
             * @brief --iter
             *
             * @return iterator&
             */
            iterator &operator--() {
                if (pos->prev == which_map->head) {
                    throw sjtu::invalid_iterator();
                }
                pos = pos->prev;
                return *this;
            }

            value_type &operator*() const {
                return *(pos->data);
            }

            /**
             * @brief check whether two iterators are same (pointing to the same memory)
             *
             * @param rhs
             * @return true
             * @return false
             */
            bool operator==(const iterator &rhs) const {
                return (which_map == rhs.which_map) && (pos == rhs.pos);
            }

            bool operator==(const const_iterator &rhs) const {
                return (which_map == rhs.which_map) && (pos == rhs.pos);
            }

            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }

            value_type *operator->() const noexcept {
                return pos->data;
            }
        };

        class const_iterator {
            friend class map;
            friend class iterator;

        private:
            const Node *pos;
            const map *which_map;

        public:
            const_iterator(const Node *pos_ = nullptr, const map *which_map_ = nullptr) : pos(pos_), which_map(which_map_) {}
            const_iterator(const const_iterator &other) : pos(other.pos), which_map(other.which_map) {}
            const_iterator(const iterator &other) : pos(other.pos), which_map(other.which_map) {}

            /**
             * @brief iter++ operation (traversal order)
             *
             * @return iterator
             */
            const_iterator operator++(int) {
                const_iterator backup = *this;
                // exception: if pos is the last element in the traversal order
                if (pos == which_map->tail) {
                    throw sjtu::invalid_iterator();
                }
                pos = pos->next;
                return backup;
            }

            /**
             * @brief ++iter operation (traversal order)
             *
             * @return iterator&
             */
            const_iterator &operator++() {
                if (pos == which_map->tail) {
                    throw sjtu::invalid_iterator();
                }
                pos = pos->next;
                return *this;
            }

            /**
             * @brief iter--
             *
             * @return iterator
             */
            const_iterator operator--(int) {
                // exception: if it is the begin, throw
                const_iterator backup = *this;
                if (pos->prev == which_map->head) {
                    throw sjtu::invalid_iterator();
                }
                pos = pos->prev;
                return backup;
            }

            /**
             * @brief --iter
             *
             * @return iterator&
             */
            const_iterator &operator--() {
                if (pos->prev == which_map->head) {
                    throw sjtu::invalid_iterator();
                }
                pos = pos->prev;
                return *this;
            }

            const value_type &operator*() const {
                return *(pos->data);
            }

            /**
             * @brief check whether the two iterators are pointing to the same memory
             *
             * @param rhs
             * @return true
             * @return false
             */
            bool operator==(const iterator &rhs) const {
                return (pos == rhs.pos) && (which_map == rhs.which_map);
            }

            bool operator==(const const_iterator &rhs) const {
                return (pos == rhs.pos) && (which_map == rhs.which_map);
            }

            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }

            const value_type *operator->() const noexcept {
                return pos->data;
            }
        };

        /**
         * @brief Construct a new map object (empty)
         *
         */
        map() {
            root = nullptr;
            current_size = 0;
            // for the head and tail pointer
            head = new Node();
            tail = new Node();
            head->next = tail;
            tail->prev = head;
        }

        map(const map &other) {
            // build the initial tree
            root = nullptr;
            head = new Node();
            tail = new Node();
            head->next = tail;
            tail->prev = head;
            root = copy(other.root);
            link_order(root, head);
            current_size = other.current_size;
        }

        map &operator=(const map &other) {
            if (this == &other) {
                return *this;
            }
            // clear the tree
            clear();
            this->root = copy(other.root);
            link_order(root, head);
            current_size = other.current_size;
            return *this;
        }

        /**
         * @brief Destroy the map object
         *
         */
        ~map() {
            clear();
            if (head != nullptr) {
                delete head;
            }
            if (tail != nullptr) {
                delete tail;
            }
        }

        /**
         * @brief access specified element with bounds checking
         *
         * @param key the specified key
         * @return T&
         */
        T &at(const Key &key) {
            Node *ans = find_val(root, key);
            if (ans == nullptr) {
                throw sjtu::index_out_of_bound();
            } else {
                return ans->data->second;
            }
        }

        /**
         * @brief const version of it
         *
         * @param key
         * @return const T&
         */
        const T &at(const Key &key) const {
            Node *ans = find_val(root, key);
            if (ans == nullptr) {
                throw sjtu::index_out_of_bound();
            } else {
                return ans->data->second;
            }
        }

        /**
         * @brief Returns a reference to the value that is mapped to a key equivalent to key, performing an insertion if such key does not already exist.
         *
         * @param key
         * @return T&
         */
        T &operator[](const Key &key) {
            Node *found = find_val(root, key);
            if (found != nullptr) {
                return found->data->second;
            }
            // insert a new value type
            value_type new_pair(key, T());
            insert(new_pair);
            // found is the current pointer of where key is found
            found = find_val(root, key);
            if (found == nullptr) {
                throw sjtu::index_out_of_bound();
            }
            return found->data->second;
        }

        /**
         * @brief behave like at() throw index_out_of_bound if such key does not exist
         *
         * @param key
         * @return const T&
         */
        const T &operator[](const Key &key) const {
            return at(key);
        }

        /**
         * @brief return the begin iterator for the map
         *
         * @return iterator
         */
        iterator begin() {
            return iterator(head->next, this);
        }

        /**
         * @brief return the const version of begin() iterator for the map
         *
         * @return const_iterator
         */
        const_iterator cbegin() const {
            return const_iterator(head->next, this);
        }

        /**
         * @brief return the end() iterator for the map
         *
         * @return iterator
         */
        iterator end() {
            return iterator(tail, this);
        }

        /**
         * @brief return cend() iterator for the map
         *
         * @return const_iterator
         */
        const_iterator cend() const {
            return const_iterator(tail, this);
        }

        /**
         * @brief check whether the map is empty
         *
         * @return true
         * @return false
         */
        bool empty() const {
            return current_size == 0;
        }

        /**
         * @brief return the number of elements
         *
         * @return size_t
         */
        size_t size() const {
            return current_size;
        }

        /**
         * @brief clear all the contents of the Tree
         *
         */
        void clear() {
            clearNode(root);
            root = nullptr;
            // reset to the empty linked list
            head->next = tail;
            tail->prev = head;
            current_size = 0;
        }

        /**
         * @brief insert an element
         *
         * @param value
         * @return pair<iterator, bool> the iterator to the new element (or the element that prevented the insertion)
         */
        pair<iterator, bool> insert(const value_type &value) {
            // do the insertion
            bool success = insert_node(value, root);
            // after the insertion, find the value
            Node *current = find_val(root, value.first);
            return pair<iterator, bool>(iterator(current, this), success);
        }

        /**
         * @brief erase the element at pos
         *
         * @param pos
         */
        void erase(iterator pos) {
            if (pos.pos == tail || root == nullptr || pos.which_map != this)
                throw sjtu::invalid_iterator();
            remove_node(pos->first, root);
        }

        /**
         * @brief Returns the number of elements with key that compares equivalent to the specified argument
         *
         * @param key
         * @return size_t
         */
        size_t count(const Key &key) const {
            return find(key) != cend() ? 1 : 0;
        }

        /**
         * @brief find a value with the equivelent key to key
         *
         * @param key
         * @return iterator (if not found, return end())
         */
        iterator find(const Key &key) {
            Node *current = find_val(root, key);
            if (current == nullptr) {
                // return end
                return end();
            } else {
                return iterator(current, this);
            }
        }

        /**
         * @brief find a value with the equivalent key to key
         *
         * @param key
         * @return const_iterator (return cend())
         */
        const_iterator find(const Key &key) const {
            Node *current = find_val(root, key);
            if (current == nullptr) {
                return cend();
            } else {
                return const_iterator(current, this);
            }
        }
    };
}// namespace sjtu
#endif
