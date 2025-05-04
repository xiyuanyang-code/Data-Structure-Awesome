
/**
 * implement a container like std::linked_hashmap
 */
 #ifndef SJTU_LINKEDHASHMAP_HPP
 #define SJTU_LINKEDHASHMAP_HPP
 
 // only for std::equal_to<T> and std::hash<T>
 #include <cstddef>
 #include <functional>
 #include <iterator>
 
 #include "Exceptions.hpp"
 #include "utility.hpp"
 
 namespace sjtu {
     /**
  * In linked_hashmap, iteration ordering is differ from map,
  * which is the order in which keys were inserted into the map.
  * You should maintain a doubly-linked list running through all
  * of its entries to keep the correct iteration order.
  *
  * Note that insertion order is not affected if a key is re-inserted
  * into the map.
  */
 
     template<class Key, class T, class Hash = std::hash<Key>, class Equal = std::equal_to<Key>>
     class linked_hashmap {
     public:
         /**
          * the internal type of data.
          * it should have a default constructor, a copy constructor.
          * You can use sjtu::linked_hashmap as value_type by typedef.
          */
         typedef pair<const Key, T> value_type;
         friend class const_iterator;
         friend class iterator;
 
     private:
         struct Node {
             value_type *data;
             Node *prev;
             Node *next;
             Node *next_in_bucket;
 
             // designed for head and tail pointer
             bool is_dummy = false;
 
 
             /**
              * @brief Construct a new Node object (for no dummy node)
              * 
              * @param val 
              * @param prev_ 
              * @param next_ 
              */
             Node(const value_type &val, Node *prev_ = nullptr, Node *next_ = nullptr, Node *next_in_bucket_ = nullptr) : prev(prev_), next(next_), next_in_bucket(next_in_bucket_) {
                 is_dummy = false;
                 data = new value_type(val);
             }
 
             /**
              * @brief Default constructor for dummy node
              * 
              * @param prev_ 
              * @param next_ 
              */
             Node(Node *prev_ = nullptr, Node *next_ = nullptr) : prev(prev_), next(next_) {
                 is_dummy = true;
                 next_in_bucket = nullptr;
                 data = nullptr;
             }
 
             ~Node() {
                 if (data != nullptr) {
                     // not the dummy node
                     delete data;
                 }
             }
         };
 
         // private data members
         Node *array;
 
         // let head and tail pointer to be the dummy node
         Node *head;
         Node *tail;
         size_t element_count;
         size_t bucket_count;
         double load_factor;
 
         // to generate the hash value for keys
         Hash hasher;
 
         // judge whether the two keys are equal
         Equal key_equal;
 
     private:
         // private function: rehash
         /**
          * @brief expand double space if the element_count is too high
          * and the load_factor is too high
          * 
          */
         void rehash() {
             bucket_count *= 2;
             // create a new array
             Node *new_array = new Node[bucket_count];
             // traverse the old array
             for (Node *it = head->next; it != tail; it = it->next) {
                 size_t position = hasher(it->data->first) % bucket_count;
                 it->next_in_bucket = new_array[position].next_in_bucket;
                 new_array[position].next_in_bucket = it;
             }
             // delete the old array
             delete[] array;
             // update the array pointer
             array = new_array;
         }
 
     public:
         /**
      * see BidirectionalIterator at CppReference for help.
      *
      * if there is anything wrong throw invalid_iterator.
      *     like it = linked_hashmap.begin(); --it;
      *       or it = linked_hashmap.end(); ++end();
      */
         class const_iterator;
         class iterator {
             friend class const_iterator;
             friend class linked_hashmap;
 
         private:
             Node *pos;
 
         public:
             // The following code is written for the C++ type_traits library.
             // Type traits is a C++ feature for describing certain properties of a
             // type. For instance, for an iterator, iterator::value_type is the type
             // that the iterator points to. STL algorithms and containers may use
             // these type_traits (e.g. the following typedef) to work properly. See
             // these websites for more information:
             // https://en.cppreference.com/w/cpp/header/type_traits
             // About value_type:
             // https://blog.csdn.net/u014299153/article/details/72419713 About
             // iterator_category: https://en.cppreference.com/w/cpp/iterator
             using difference_type = std::ptrdiff_t;
             using value_type = typename linked_hashmap::value_type;
             using pointer = value_type *;
             using reference = value_type &;
             using iterator_category = std::output_iterator_tag;
 
             /**
              * @brief Construct a new iterator object (an empty iterator)
              * 
              */
             iterator() {
                 pos = nullptr;
             }
 
             /**
              * @brief Construct a new iterator object
              * 
              * @param other 
              */
             iterator(const iterator &other) {
                 pos = other.pos;
             }
 
             /**
              * @brief Construct a new iterator object (when given a node pointer)
              * 
              * @param node_pos 
              */
             iterator(Node *node_pos) {
                 pos = node_pos;
             }
 
             iterator(const const_iterator &it) {
                 pos = it.pos;
             }
 
 
             /**
              * @brief iter++ operation
              * 
              * @return iterator 
              * @exception invalid_iterator() when the ++ operation is invalid
              */
             iterator operator++(int) {
                 if (pos == nullptr || pos->next == nullptr) {
                     throw sjtu::invalid_iterator();
                 }
 
                 iterator tmp = *this;
                 pos = pos->next;
                 return tmp;
             }
 
 
             /**
              * @brief ++iter operation
              * 
              * @return iterator 
              * @exception invalid_iterator() when the ++ operation is invalid
              */
             iterator &operator++() {
                 if (pos == nullptr || pos->next == nullptr) {
                     throw sjtu::invalid_iterator();
                 }
 
                 pos = pos->next;
                 return *this;
             }
 
             /**
              * @brief iter-- operation
              * 
              * @return iterator 
              * @exception invalid_iterator() when the -- operation is invalid
              */
             iterator operator--(int) {
                 if (pos == nullptr || pos->prev == nullptr || pos->prev->prev == nullptr) {
                     // !unlike tail, head pointer is banned in the iterator
                     throw sjtu::invalid_iterator();
                 }
 
                 iterator tmp = *this;
                 pos = pos->prev;
                 return tmp;
             }
 
             /**
              * @brief --iter operation
              * 
              * @return iterator 
              * @exception invalid_iterator() when the -- operation is invalid
              */
             iterator &operator--() {
                 if (pos == nullptr || pos->prev == nullptr || pos->prev->prev == nullptr) {
                     // !unlike tail, head pointer is banned in the iterator
                     throw sjtu::invalid_iterator();
                 }
 
                 pos = pos->prev;
                 return *this;
             }
 
 
             /**
              * @brief return the value type
              * 
              * @return value_type& 
              */
             value_type &operator*() const {
                 return *(pos->data);
             }
 
             /**
              * @brief judge whether two iterators point at the same memory (for data members)
              * 
              * @param rhs 
              * @return true 
              * @return false 
              */
             bool operator==(const iterator &rhs) const {
                 return (this->pos == rhs.pos);
             }
             bool operator==(const const_iterator &rhs) const {
                 return (this->pos == rhs.pos);
             }
             bool operator!=(const iterator &rhs) const {
                 return (this->pos != rhs.pos);
             }
             bool operator!=(const const_iterator &rhs) const {
                 return (this->pos != rhs.pos);
             }
 
 
             value_type *operator->() const noexcept {
                 return pos->data;
             }
         };
 
         class const_iterator {
             friend class iterator;
             friend class linked_hashmap;
 
         private:
             const Node *pos;
 
         public:
             /**
              * @brief Construct a new const_iterator object (an empty iterator)
              * 
              */
             const_iterator() {
                 pos = nullptr;
             }
 
             /**
              * @brief Construct a new const_iterator object
              * 
              * @param other 
              */
             const_iterator(const const_iterator &other) {
                 pos = other.pos;
             }
 
             /**
              * @brief Construct a new const_iterator object (when given a node pointer)
              * 
              * @param node_pos 
              */
             const_iterator(Node *node_pos) {
                 pos = node_pos;
             }
 
             const_iterator(const iterator &it) {
                 pos = it.pos;
             }
 
 
             /**
              * @brief iter++ operation
              * 
              * @return iterator 
              * @exception invalid_iterator() when the ++ operation is invalid
              */
             const_iterator operator++(int) {
                 if (pos == nullptr || pos->next == nullptr) {
                     throw sjtu::invalid_iterator();
                 }
 
                 const_iterator tmp = *this;
                 pos = pos->next;
                 return tmp;
             }
 
 
             /**
              * @brief ++iter operation
              * 
              * @return iterator 
              * @exception invalid_iterator() when the ++ operation is invalid
              */
             const_iterator &operator++() {
                 if (pos == nullptr || pos->next == nullptr) {
                     throw sjtu::invalid_iterator();
                 }
 
                 pos = pos->next;
                 return *this;
             }
 
             /**
              * @brief iter-- operation
              * 
              * @return iterator 
              * @exception invalid_iterator() when the -- operation is invalid
              */
             const_iterator operator--(int) {
                 if (pos == nullptr || pos->prev == nullptr || pos->prev->prev == nullptr) {
                     // !unlike tail, head pointer is banned in the iterator
                     throw sjtu::invalid_iterator();
                 }
 
                 const_iterator tmp = *this;
                 pos = pos->prev;
                 return tmp;
             }
 
             /**
              * @brief --iter operation
              * 
              * @return iterator 
              * @exception invalid_iterator() when the -- operation is invalid
              */
             const_iterator &operator--() {
                 if (pos == nullptr || pos->prev == nullptr || pos->prev->prev == nullptr) {
                     // !unlike tail, head pointer is banned in the iterator
                     throw sjtu::invalid_iterator();
                 }
 
                 pos = pos->prev;
                 return *this;
             }
 
 
             /**
              * @brief return the value type
              * 
              * @return value_type& 
              */
             const value_type &operator*() const {
                 return *(pos->data);
             }
 
             /**
              * @brief judge whether two iterators point at the same memory(for data members)
              * 
              * @param rhs 
              * @return true 
              * @return false 
              */
             bool operator==(const iterator &rhs) const {
                 return (this->pos == rhs.pos);
             }
             bool operator==(const const_iterator &rhs) const {
                 return (this->pos == rhs.pos);
             }
             bool operator!=(const iterator &rhs) const {
                 return (this->pos != rhs.pos);
             }
             bool operator!=(const const_iterator &rhs) const {
                 return (this->pos != rhs.pos);
             }
 
 
             value_type *operator->() const noexcept {
                 return pos->data;
             }
         };
 
         /**
          * @brief Construct a new linked hashmap object
          * 
          */
         linked_hashmap() {
             element_count = 0;
 
 
             // create dummy node
             head = new Node;
             tail = new Node;
             head->next = tail;
             tail->prev = head;
 
             // add load_factor
             load_factor = 0.75;
             bucket_count = 12;
             array = new Node[bucket_count];
         }
 
         /**
          * @brief Construct a new linked hashmap object(copy constructor)
          * 
          * @param other 
          */
         linked_hashmap(const linked_hashmap &other) {
             // create an empty linked_hashmap first
             element_count = other.element_count;
 
             head = new Node;
             tail = new Node;
             head->next = tail;
             tail->prev = head;
 
             load_factor = 0.75;
             bucket_count = 12;
 
             // specify the correct bucket count
             while (bucket_count * load_factor <= element_count) {
                 bucket_count *= 2;
             }
             array = new Node[bucket_count];
             // hasher = other.hasher;
             // key_equal = other.key_equal;
             element_count = 0;
 
             // traverse
             for (Node *it = other.head->next; it != other.tail; it = it->next) {
                 insert(*(it->data));
             }
         }
 
         /**
          * @brief Assignment operator for linked_hashmap
          * 
          * @param other 
          * @return linked_hashmap& 
          */
         linked_hashmap &operator=(const linked_hashmap &other) {
             if (&other == this) {
                 return *this;
             }
 
             // clear the linked_hashmap, reset to the empty statement
             clear();
 
             if (other.element_count == 0) {
                 // remain the empty statement
                 return *this;
             }
             delete[] array;
 
             // hasher = other.hasher;
             // key_equal = other.key_equal;
             element_count = other.element_count;
             tail->prev = head;
             head->next = tail;
 
             while (bucket_count * load_factor <= element_count) {
                 bucket_count *= 2;
             }
             array = new Node[bucket_count];
             element_count = 0;
 
             // traverse
             for (Node *it = other.head->next; it != other.tail; it = it->next) {
                 insert(*(it->data));
             }
             return *this;
         }
 
         /**
          * @brief Destroy the linked hashmap object
          * 
          */
         ~linked_hashmap() {
             // reset to the empty statement
             clear();
 
             // delete all the dummy node
             delete[] array;
             delete head;
             delete tail;
         }
 
         /**
          * @brief access specified element with bounds checking
          * 
          * @param key the given key
          * @return T& Returns a reference to the mapped value of the element with key equivalent to key.
          */
         T &at(const Key &key) {
             size_t mapped_value = hasher(key) % bucket_count;
             Node &bucket_position = array[mapped_value];
 
             for (Node *it = array[mapped_value].next_in_bucket; it != nullptr; it = it->next_in_bucket) {
                 if (key_equal(it->data->first, key)) {
                     // find the value
                     return it->data->second;
                 }
             }
 
             // if not find, throw the error
             throw sjtu::index_out_of_bound();
         }
 
         /**
          * @brief const version of the at function
          * 
          * @param key 
          * @return const T& 
          */
         const T &at(const Key &key) const {
             size_t mapped_value = hasher(key) % bucket_count;
             Node &bucket_position = array[mapped_value];
 
             for (Node *it = array[mapped_value].next_in_bucket; it != nullptr; it = it->next_in_bucket) {
                 if (key_equal(it->data->first, key)) {
                     // find the value
                     return it->data->second;
                 }
             }
 
             // if not find, throw the error
             throw sjtu::index_out_of_bound();
         }
 
         /**
          * @brief access specified element
          * 
          * @param key 
          * @return T& Returns a reference to the value that is mapped to a key equivalent to key, performing an insertion if such key does not already exist.
          */
         T &operator[](const Key &key) {
             if (element_count >= bucket_count * load_factor) {
                 rehash();
             }
             size_t mapped_value = hasher(key) % bucket_count;
             Node &bucket_position = array[mapped_value];
 
 
             for (Node *it = bucket_position.next_in_bucket; it != nullptr; it = it->next_in_bucket) {
                 // for the insertion, we need to store the last element in the linked list
 
                 if (key_equal(it->data->first, key)) {
                     // find the value
                     return it->data->second;
                 }
             }
 
             // if not find, do the insertion
             auto inserted_value = value_type(key, T());
             insert(inserted_value);
             mapped_value = hasher(key) % bucket_count;
             return array[mapped_value].next_in_bucket->data->second;
         }
 
         /**
          * @brief behave like at() throw index_out_of_bound if such key does not exist.
          * 
          * @param key 
          * @return const T& 
          */
         const T &operator[](const Key &key) const {
             size_t mapped_value = hasher(key) % bucket_count;
             Node &bucket_position = array[mapped_value];
 
             for (Node *it = bucket_position.next_in_bucket; it != nullptr; it = it->next_in_bucket) {
                 if (key_equal(it->data->first, key)) {
                     // find the value
                     return it->data->second;
                 }
             }
 
             // if not find, throw the error
             throw sjtu::index_out_of_bound();
         }
 
         /**
          * @brief return a iterator to the beginning
          * ! head pointer is not allowed
          * 
          * @return iterator 
          */
         iterator begin() {
             return iterator(head->next);
         }
 
         /**
          * @brief return a const iterator to the beginning
          * ! head pointer is not allowed
          * 
          * @return iterator 
          */
         const_iterator cbegin() const {
             return const_iterator(head->next);
         }
 
         /**
          * @brief return a iterator to the end
          * 
          * @return iterator 
          */
         iterator end() {
             return iterator(tail);
         }
 
         /**
          * @brief return a const_iterator to the end
          * 
          * @return const_iterator 
          */
         const_iterator cend() const {
             return const_iterator(tail);
         }
 
         /**
          * @brief check whether the container is empty
          * 
          * @return true 
          * @return false 
          */
         bool empty() const {
             return element_count == 0;
         }
 
         /**
          * @brief return the number of the element
          * 
          * @return size_t 
          */
         size_t size() const {
             return element_count;
         }
 
 
         /**
          * @brief clear all the element, reset to the empty statement
          * 
          */
         void clear() {
             // reset to the empty statement
             element_count = 0;
             for (Node *it = head->next; it != tail;) {
                 auto storage = it->next;
                 delete it;
                 it = storage;
             }
 
             // reset the status of dummy node
             head->next = tail;
             tail->prev = head;
 
             for (size_t i = 0; i < bucket_count; i++) {
                 array[i].next_in_bucket = nullptr;
             }
         }
 
 
         /**
          * @brief insert an element
          * 
          * @param value 
          * @return pair<iterator, bool> :return a pair, the first of the pair is the iterator to the new element (or the element that prevented the insertion), the second one is true if insert successfully, or false.
          */
         pair<iterator, bool> insert(const value_type &value) {
             if (element_count >= bucket_count * load_factor) {
                 rehash();
             }
             auto position = hasher(value.first) % bucket_count;
             Node &bucket_position = array[position];
 
             // check whether it has been inserted
             for (Node *it = bucket_position.next_in_bucket; it != nullptr; it = it->next_in_bucket) {
                 if (key_equal(it->data->first, value.first)) {
                     // find the value, return false
                     return pair<iterator, bool>(iterator(it), false);
                     // !attention, it will not affect the T value!
                 }
             }
 
 
             position = hasher(value.first) % bucket_count;
             // for the hash table, do the insertion
             array[position].next_in_bucket = new Node(value, tail->prev, tail, array[position].next_in_bucket);
 
             // reset the tail pointer
             tail->prev->next = array[position].next_in_bucket;
             tail->prev = array[position].next_in_bucket;
             element_count++;
             return pair<iterator, bool>(iterator(array[position].next_in_bucket), true);
         }
 
 
         /**
          * @brief erase the element at pos.
          * 
          * @param pos 
          * @throw  throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
          */
         void erase(iterator pos) {
             // if pos pointed to a bad element
             if (pos == end() || pos.pos == nullptr || pos.pos->data == nullptr) {
                 throw sjtu::index_out_of_bound();
             }
 
             size_t position = hasher(pos.pos->data->first) % bucket_count;
             Node &bucket_position = array[position];
             Node *storage = array + position;
 
             for (Node *it = bucket_position.next_in_bucket; it != nullptr; storage = it, it = it->next_in_bucket) {
                 if (pos.pos == it) {
                     // find the same value! (memory)
                     it->next->prev = it->prev;
                     it->prev->next = it->next;
                     storage->next_in_bucket = it->next_in_bucket;
                     element_count--;
                     delete it;
                     return;
                 }
             }
 
             // if we donot find the element, throw
             throw sjtu::index_out_of_bound();
         }
 
 
         /**
          * @brief Returns the number of elements with key that compares equivalent to the specified argument.
          * 
          * @param key 
          * @return size_t 0 ir 1
          */
         size_t count(const Key &key) const {
             auto position = hasher(key) % bucket_count;
             Node &bucket_position = array[position];
             for (Node *it = bucket_position.next_in_bucket; it != nullptr; it = it->next_in_bucket) {
                 if (key_equal(it->data->first, key)) {
                     return 1;
                 }
             }
             return 0;
         }
 
 
         /**
          * @brief Finds an element with key equivalent to key.
          * 
          * @param key 
          * @return iterator Iterator to an element with key equivalent to key.
          */
         iterator find(const Key &key) {
             size_t position = hasher(key) % bucket_count;
             Node &bucket_position = array[position];
 
             for (Node *it = bucket_position.next_in_bucket; it != nullptr; it = it->next_in_bucket) {
                 if (key_equal(it->data->first, key)) {
                     return iterator(it);
                 }
             }
             return end();
         }
 
         /**
          * @brief the const version of find
          * 
          * @param key 
          * @return const_iterator 
          */
         const_iterator find(const Key &key) const {
             size_t position = hasher(key) % bucket_count;
             Node bucket_position = array[position];
 
             for (Node *it = bucket_position.next_in_bucket; it != nullptr; it = it->next_in_bucket) {
                 if (key_equal(it->data->first, key)) {
                     return const_iterator(it);
                 }
             }
             return cend();
         }
     };
 
 }// namespace sjtu
 
 #endif