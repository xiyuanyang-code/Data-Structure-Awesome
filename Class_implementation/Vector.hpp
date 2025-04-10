/*
remember to use c++20 while compiling
*/

#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "Exceptions.hpp"
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstring>
#include <memory>

namespace sjtu {

    /**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
    template<typename T>
    class vector {
    private:
        T *data;            // for data storage
        size_t current_size;// the size of vector
        size_t max_size;    // default value is 16

        /**
     * @brief expand the memory (the maxsize and data) to its double size
     * 
     * just expand the memory without allocating new members
     */
        void expandDouble() {
            std::allocator<T> alloc;
            size_t new_max_size = max_size * 2;
            T *new_data = alloc.allocate(new_max_size);

            // move to the new memory space
            for (size_t i = 0; i < current_size; ++i) {
                std::construct_at(new_data + i, std::move(data[i]));
                std::destroy_at(data + i);
            }

            // release old memory
            if (data) {
                alloc.deallocate(data, max_size);
            }

            // update the pointer
            data = new_data;
            new_data = nullptr;
            max_size = new_max_size;
        }

    public:
        /**
     * @brief Tool function, delete all the elements but does not free the memory
     */
        void clear() {
            std::allocator<T> alloc;
            for (size_t i = 0; i < current_size; ++i) {
                std::destroy_at(data + i);
            }
            current_size = 0;
        }

    public:
        class const_iterator;

        class iterator {
            // The following code is written for the C++ type_traits library.
            // Type traits is a C++ feature for describing certain properties of a type.
            // For instance, for an iterator, iterator::value_type is the type that the
            // iterator points to.
            // STL algorithms and containers may use these type_traits (e.g. the following
            // typedef) to work properly. In particular, without the following code,
            // @code{std::sort(iter, iter1);} would not compile.
            // See these websites for more information:
            // https://en.cppreference.com/w/cpp/header/type_traits
            // About value_type: https://blog.csdn.net/u014299153/article/details/72419713
            // About iterator_category: https://en.cppreference.com/w/cpp/iterator
        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;
            using iterator_category = std::output_iterator_tag;

        private:
            T *ptr;                 // the pointer
            vector<T> *which_vector;// judge whether two pointers are pointing to the same vector

        public:
            // constructor
            iterator(T *ptr_, vector<T> *which_vec) : ptr(ptr_), which_vector(which_vec) {}

            iterator operator+(const int &n) const {
                iterator tmp = iterator(ptr + n, this->which_vector);
                if (tmp < (*which_vector).begin() || (*which_vector).end() < tmp) {
                    throw invalid_iterator();
                }
                return tmp;
            }

            iterator operator-(const int &n) const {
                iterator tmp = iterator(ptr - n, this->which_vector);
                if (tmp < (*which_vector).begin() || (*which_vector).end() < tmp) {
                    throw invalid_iterator();
                }
                return tmp;
            }

            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invalid_iterator.
            int operator-(const iterator &rhs) const {
                if (which_vector != rhs.which_vector) {
                    // two iterators point to different vectors!
                    throw invalid_iterator();
                }
                return ptr - rhs.ptr;
            }

            /**
         * @brief let the iterator to move towards n steps
         * 
         * @param n 
         * @return iterator& 
         */
            iterator &operator+=(const int &n) {
                ptr += n;// movement
                if (*this < (*which_vector).begin() || (*which_vector).end() < *this) {
                    throw invalid_iterator();
                }
                return *this;
            }

            iterator &operator-=(const int &n) {
                ptr = ptr - n;
                if (*this < (*which_vector).begin() || (*which_vector).end() < *this) {
                    throw invalid_iterator();
                }
                return *this;
            }

            iterator operator++(int) {
                iterator temp = *this;
                ++ptr;
                return temp;
            }

            iterator &operator++() {
                ++ptr;
                if (*this < (*which_vector).begin() || (*which_vector).end() < *this) {
                    throw invalid_iterator();
                }
                return *this;
            }

            iterator operator--(int) {
                iterator tmp = *this;
                --ptr;
                return tmp;
            }

            iterator operator--() {
                --ptr;
                if (*this < (*which_vector).begin() || (*which_vector).end() < *this) {
                    throw invalid_iterator();
                }
                return *this;
            }

            T &operator*() const {
                return *ptr;
            }

            bool operator==(const iterator &rhs) const {
                return (which_vector == rhs.which_vector) && (ptr == rhs.ptr);
            }

            bool operator==(const const_iterator &rhs) const {
                return (which_vector == rhs.which_vector) && (ptr == rhs.ptr);
            }

            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }

            bool operator<(const iterator &rhs) const {
                return (*this - rhs) < 0;
            }
        };

        class const_iterator {
        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;
            using iterator_category = std::output_iterator_tag;

        private:
            const T *ptr;
            const vector<T> *which_vector;

        public:
            const_iterator(const T *p, const vector<T> *which_vec) : ptr(p), which_vector(which_vec) {}

            const_iterator operator+(const int n) const {
                return const_iterator(ptr + n);
            }

            const_iterator operator-(const int n) const {
                return const_iterator(ptr - n);
            }

            int operator-(const const_iterator &rhs) const {
                if (which_vector != rhs.which_vector) {
                    throw invalid_iterator();
                } else {
                    return ptr - rhs.ptr;
                }
            }

            const_iterator &operator+=(const int &n) {
                ptr = ptr + n;
                return *this;
            }

            const_iterator &operator-=(const int &n) {
                ptr = ptr - n;
                return *this;
            }

            const_iterator operator++(int) {
                const_iterator temp = *this;
                ++ptr;
                return temp;
            }

            const_iterator &operator++() {
                ++ptr;
                return *this;
            }

            const_iterator operator--() {
                --ptr;
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator tmp = *this;
                --ptr;
                return tmp;
            }

            T operator*() const {
                return *ptr;
            }

            bool operator==(const const_iterator &rhs) const {
                return ptr == rhs.ptr;
            }

            bool operator==(const iterator &rhs) const {
                return ptr == rhs.ptr;
            }

            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }
        };

        /**
     * @brief Construct a new vector object (default)
     */
        vector() : current_size(0), max_size(16) {
            // default
            std::allocator<T> alloc;
            data = alloc.allocate(max_size);
        }

        /**
     * @brief Construct a new vector object
     * 
     * @param size_
     */
        vector(size_t size_) {
            current_size = 0;
            max_size = (current_size < 16) ? 16 : current_size;
            std::allocator<T> alloc;
            data = alloc.allocate(max_size);
        }

        vector(const vector &other) {
            current_size = other.current_size;
            max_size = other.max_size;
            std::allocator<T> alloc;
            data = alloc.allocate(max_size);
            for (int i = 0; i < current_size; ++i) {
                std::construct_at(data + i, other.data[i]);
            }
        }

        ~vector() {
            clear();// destroy all data, not release memory
            if (data) {
                std::allocator<T> alloc;
                alloc.deallocate(data, max_size);
            }
        }

        /**
     * @brief assignment operator
     * 
     * @param other 
     * @return vector& the assigned operator
     */
        vector<T> &operator=(const vector &other) {
            if (this != &other) {
                clear();
                std::allocator<T> alloc;
                if (data) {
                    alloc.deallocate(data, max_size);
                }
                data = alloc.allocate(other.max_size);
                max_size = other.max_size;
                current_size = other.current_size;
                for (size_t i = 0; i < current_size; ++i) {
                    std::construct_at(data + i, other.data[i]);
                }
            }
            return *this;
        }

        /**
     * assigns specified element with bounds checking
     * throw index_out_of_bound if pos is not in [0, size)
     */
        T &at(const size_t &pos) {
            if (pos < 0 || pos >= current_size) {
                throw index_out_of_bound();
            } else {
                return data[pos];
            }
        }

        const T &at(const size_t &pos) const {
            if (pos < 0 || pos >= current_size) {
                throw index_out_of_bound();
            } else {
                return data[pos];
            }
        }

        /**
     * assigns specified element with bounds checking
     * throw index_out_of_bound if pos is not in [0, size)
     * !!! Pay attentions
     *   In STL this operator does not check the boundary but I want you to do.
     */
        T &operator[](const size_t &pos) {
            if (pos < 0 && pos >= current_size) {
                throw index_out_of_bound();
            } else {
                return data[pos];
            }
        }

        const T &operator[](const size_t &pos) const {
            if (pos < 0 && pos >= current_size) {
                throw index_out_of_bound();
            } else {
                return data[pos];
            }
        }

        /**
     * access the first element.
     * throw container_is_empty if size == 0
     */
        const T &front() const {
            if (current_size == 0) {
                throw container_is_empty();
            } else {
                return data[0];
            }
        }

        /**
     * access the last element.
     * throw container_is_empty if size == 0
     */
        const T &back() const {
            if (current_size == 0) {
                throw container_is_empty();
            } else {
                return data[current_size - 1];
            }
        }

        /**
     * returns an iterator to the beginning.
     */
        iterator begin() {
            if (empty()) {
                throw container_is_empty();
            }
            iterator tmp(data, this);
            return tmp;
        }

        const_iterator begin() const {
            if (empty()) {
                throw container_is_empty();
            } else {
                const_iterator tmp(data, this);
                return tmp;
            }
        }

        const_iterator cbegin() const {
            if (empty()) {
                throw container_is_empty();
            } else {
                const_iterator tmp(data, this);
                return tmp;
            }
        }

        /**
     * returns an iterator to the end.
     */
        iterator end() {
            if (empty()) {
                throw container_is_empty();
            } else {
                iterator tmp(data + current_size, this);
                return tmp;
            }
        }

        const_iterator end() const {
            if (empty()) {
                throw container_is_empty();
            } else {
                const_iterator tmp(data + current_size, this);
                return tmp;
            }
        }

        const_iterator cend() const {
            if (empty()) {
                throw container_is_empty();
            } else {
                const_iterator tmp(data + current_size, this);
                return tmp;
            }
        }

        /**
     * checks whether the container is empty
     */
        bool empty() const {
            return current_size == 0;
        }

        /**
     * returns the number of elements
     */
        size_t size() const {
            return current_size;
        }

        /**
     * inserts value before pos
     * returns an iterator pointing to the inserted value.
     */
        iterator insert(iterator pos, const T &value) {
            // getting the inserted index
            const size_t index = pos - (this->begin());
            return insert(index, value);
        }

        /**
     * inserts value at index ind.
     * after inserting, this->at(ind) == value
     * returns an iterator pointing to the inserted value.
     * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
     */
        iterator insert(const size_t &pos, const T &value) {
            //check if it is valid
            if (pos > current_size) {
                throw index_out_of_bound();
            }
            //expand memory
            if (current_size == max_size) {
                expandDouble();
            }
            std::construct_at(data + current_size, value);

            //move backwards
            for (size_t i = current_size; i > pos; --i) {
                data[i] = std::move(data[i - 1]);
            }

            //create a new element
            data[pos] = value;
            ++current_size;

            return begin() + pos;
        }

        /**
     * removes the element at pos.
     * return an iterator pointing to the following element.
     * If the iterator pos refers the last element, the end() iterator is returned.
     */
        iterator erase(iterator pos) {
            if (pos == end()) {
                // like the push back
                return end();
            }
            iterator next_pos = pos + 1;
            std::move(next_pos, end(), pos);

            // update the vector
            std::destroy_at(data + current_size - 1);
            --current_size;
            return pos;
        }

        /**
     * removes the element with index ind.
     * return an iterator pointing to the following element.
     * throw index_out_of_bound if ind >= size
     */
        iterator erase(const size_t &ind) {
            if (ind >= current_size) {
                throw index_out_of_bound();
            } else {
                std::move(data + ind + 1, data + current_size, data + ind);
                std::destroy_at(data + current_size - 1);
                --current_size;
                return this->begin() + ind;
            }
        }

        /**
     * adds an element to the end.
     */
        void push_back(const T &value) {
            if (current_size == max_size) {
                // needs to expand
                expandDouble();
            }
            std::construct_at(data + current_size, value);
            ++current_size;
        }

        /**
     * remove the last element from the end.
     * throw container_is_empty if size() == 0
     */
        void pop_back() {
            if (size() == 0) {
                throw container_is_empty();
            } else {
                std::destroy_at(data + current_size - 1);
                --current_size;
            }
        }
    };

}// namespace sjtu

#endif