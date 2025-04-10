/*
 * @Author: Xiyuan Yang   xiyuan_yang@outlook.com
 * @Date: 2025-04-03 14:10:59
 * @LastEditors: Xiyuan Yang   xiyuan_yang@outlook.com
 * @LastEditTime: 2025-04-10 13:16:51
 * @FilePath: /Data_structure/Class_implementation/BST.cpp
 * @Description: Dynamic Search Table
 * Do you code and make progress today?
 * Copyright (c) 2025 by Xiyuan Yang, All Rights Reserved.
 */

#include <cstdio>
#include <iostream>

template<class KEY, class OTHER>
struct set {
		KEY key;
		OTHER other;
};

// Dynamic Search Table
template<class Key, class Other>
class dynamicSearchTable {
public:
		// finding elements of x
		virtual set<Key, Other> *find(const Key &x) const = 0;

		// insert element, maintaining the order
		virtual void insert(const set<Key, Other> &x) = 0;

		// remove an element, maintaining the order
		virtual void remove(const Key &x) = 0;

		virtual ~dynamicSearchTable() {}
};

template<class Key, class Other>
class BST : public dynamicSearchTable<Key, Other> {
private:
		struct BinaryNode {
				set<Key, Other> data;
				BinaryNode *left;
				BinaryNode *right;

				BinaryNode(const set<Key, Other> &data_, BinaryNode *left_ = nullptr,
									 BinaryNode *right_ = nullptr)
						: data(data_), left(left_), right(right_) {}
		};

		// root node
		BinaryNode *root;

private:
		// tool function

		/**
   * @brief insert the struct into the binary tree
   *
   * @param x
   * @param t the root node (for recursion)
   */
		void insert(const set<Key, Other> &x, BinaryNode *&t) {
				if (t == nullptr) {
						// Adding a new element on the leaf node
						t = new BinaryNode(x, nullptr, nullptr);
				} else if (x.key < t->data.key) {
						insert(x, t->left);
				} else if (x.key > t->data.key) {
						insert(x, t->right);
				}
				// If x.key = t -> data.key, won't do anything
		}

		/**
   * @brief remove element with the key value of x
   *
   * @param x
   * @param t the root node
   */
		void remove(const Key &x, BinaryNode *&t) {
				// if nullptr, return and end the recursion
				if (t == nullptr) {
						return;
				}

				if (x < t->data.key) {
						remove(x, t->left);
				} else if (x > (t->data.key)) {
						remove(x, t->right);
				}
				// If we find the element...
				// If it has one child:just replace it with is child
				// If it has zero child: just delete it
				else {
						if (t->left != nullptr && t->right != nullptr) {
								BinaryNode *tmp = t->right;
								while (tmp->left != nullptr) {
										// traverse the next element in the traversal order
										tmp = tmp->left;
								}
								t->data = tmp->data;
								remove(t->data.key, t->right);
						} else {
								BinaryNode *oldnode = t;
								// If it has one child, replace it
								t = (t->left != nullptr) ? t->left : t->right;
								delete oldnode;
						}
				}
		}

		/**
   * @brief Make the tree empty
   *
   * @param t the root node
   */
		void makeEmpty(BinaryNode *t) {
				if (t == nullptr) {
						return;
				}

				makeEmpty(t->left);
				makeEmpty(t->right);
				delete t;
		}

		/**
   * @brief find the specific element using the recursion
   *
   * @param x key value
   * @param t root node
   * @return set<Key, Other>*
   */
		set<Key, Other> *find(const Key &x, BinaryNode *t) const {
				if (t == nullptr) {
						// goto the end (leaf node)
						return nullptr;
				}

				// finding the element successfully
				if (t->data.key == x) {
						// successfully
						return (set<Key, Other> *) t;
				}

				// recursively searching for the left & right subtree
				if (x < t->data.key) {
						return find(x, t->left);
				} else {
						return find(x, t->right);
				}
		}

public:
		BST() {
				// create an empty tree
				root = nullptr;
		}

		~BST() { makeEmpty(root); }

		/**
   * @brief Insert value x in the BST
   *
   * @param x
   */
		void insert(const set<Key, Other> &x) { insert(x, root); }

		/**
   * @brief remove the value with the key value of x
   *
   */
		void remove(const Key &x) { remove(x, root); }

		/**
   * @brief find the element with the key value of x
   *
   * @param x
   * @return set<Key, Other>*
   */
		set<Key, Other> *find(const Key &x) const { return find(x, root); }
};

int main() {
		// 创建一个BST实例
		BST<int, std::string> bst;

		// 插入一些元素
		set<int, std::string> elements[] = {
						{10, "Alice"},
						{5, "Bob"},
						{15, "Charlie"},
						{3, "David"},
						{7, "Eve"},
						{12, "Frank"},
						{18, "Grace"}};

		std::cout << "Inserting elements into the BST:" << std::endl;
		for (const auto &elem : elements) {
				bst.insert(elem);
				std::cout << "Inserted: Key = " << elem.key << ", Value = " << elem.other
									<< std::endl;
		}

		// 测试查找功能
		std::cout << "\nTesting find function:" << std::endl;
		int keysToFind[] = {10, 5, 15, 100};// 100 is not in the tree
		for (int key : keysToFind) {
				set<int, std::string> *result = bst.find(key);
				if (result) {
						std::cout << "Found: Key = " << result->key
											<< ", Value = " << result->other << std::endl;
				} else {
						std::cout << "Key " << key << " not found." << std::endl;
				}
		}

		// 测试删除功能
		std::cout << "\nTesting remove function:" << std::endl;
		int keysToRemove[] = {5, 15, 100};// 100 is not in the tree
		for (int key : keysToRemove) {
				std::cout << "Removing key " << key << "..." << std::endl;
				bst.remove(key);

				// 验证删除后的查找结果
				set<int, std::string> *result = bst.find(key);
				if (result) {
						std::cout << "Key " << key << " still exists after removal." << std::endl;
				} else {
						std::cout << "Key " << key << " successfully removed." << std::endl;
				}
		}

		// 再次测试查找功能以确认删除效果
		std::cout << "\nRe-testing find function after removals:" << std::endl;
		for (int key : keysToFind) {
				set<int, std::string> *result = bst.find(key);
				if (result) {
						std::cout << "Found: Key = " << result->key
											<< ", Value = " << result->other << std::endl;
				} else {
						std::cout << "Key " << key << " not found." << std::endl;
				}
		}

		return 0;
}