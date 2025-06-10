# Hand-Made Data Structure: SJTU-CS0501H STLite Awesome 👋

## Introduction

Welcome to STLite—a personal passion project where I implement fundamental data structures and algorithms from scratch. This repository serves as a lightweight, educational library designed to deepen my understanding of core computer science concepts.

It also functions as **lecture notes and assignments** for **SJTU’s CS-0501H course**. Feel free to use this code as a reference for your own data structures learning journey!😊😊😊

---

## Table of Contents

### Algorithms

This section is a comprehensive collection of classic algorithms, ranging from graph traversals to sorting techniques, all implemented in C++. Each file represents a direct and often optimized approach to solving common computational problems.

-   `BFS.cpp`: Explore the graph layer by layer with the **Breadth-First Search (BFS)** algorithm. This implementation is crucial for finding the shortest path in unweighted graphs and for network traversal problems.

-   `DAG_SSSP.cpp`: Discover the **Single Source Shortest Path (SSSP)** in **Directed Acyclic Graphs (DAGs)**. This specialized algorithm leverages the topological order of a DAG to efficiently find shortest paths, often outperforming general shortest path algorithms like Dijkstra's on such graphs.

-   `DFS.cpp`: Delve deep into graph structures using the **Depth-First Search (DFS)** algorithm. This versatile implementation includes functionalities for generating DFS order (when nodes are first visited) and Euler order (a tour of the graph's edges), vital for many graph-related tasks like topological sorting and cycle detection.

-   `Eulerian_path.cpp`: Find your way through graphs with the **Eulerian Path** algorithm. This implementation determines if an Eulerian path or circuit exists within a graph and, if so, constructs one, allowing you to traverse every edge exactly once.

-   `Floyd.cpp`: Solve the **All-Pairs Shortest Path** problem with the **Floyd-Warshall algorithm**. This dynamic programming approach efficiently computes the shortest paths between all pairs of vertices in a weighted graph, making it invaluable for scenarios where global shortest path information is needed.

-   `Josephus.cpp`: Tackle the classic **Josephus Problem** using a circular linked list. This implementation simulates the ancient problem of survival by elimination in a circle, demonstrating efficient manipulation of circular data structures. (==To be done in the future==)

-   `Kruskal.cpp`: Construct the **Minimum Spanning Tree (MST)** of a graph with **Kruskal's algorithm**. This greedy algorithm builds the MST by iteratively adding the cheapest edge that does not form a cycle, making it efficient for sparse graphs.

-   `LCA.cpp`: Efficiently find the **Lowest Common Ancestor (LCA)** of two nodes in a tree using **binary lifting**. This technique preprocesses the tree to answer LCA queries rapidly, critical for problems involving tree paths and relationships.

-   `LCA2.cpp`: An alternative implementation for finding the **Lowest Common Ancestor (LCA)**.

-   `LCA3.cpp`: An alternative implementation for finding the **Lowest Common Ancestor (LCA)**.

-   `Maxsum_of_array.cpp`: Uncover the maximum contiguous subarray sum with **Kadane's algorithm**. This efficient dynamic programming approach finds the subarray within a one-dimensional array of numbers that has the largest sum, a common interview question and practical problem.

-   `Prim.cpp`: Build the **Minimum Spanning Tree (MST)** using **Prim's algorithm**. This greedy algorithm grows the MST by adding the cheapest edge from the current tree to an unvisited vertex, often preferred for dense graphs.

-   `Segment_Tree.cpp`: Master range queries and updates with the **Segment Tree** data structure. This powerful tree-based structure efficiently handles operations on intervals or segments, such as range sum queries or range minimum queries.

-   `Sorting.cpp`: A comprehensive collection of various **sorting algorithms**. This file includes implementations of popular sorting techniques like Bubble Sort, Insertion Sort, Merge Sort, Quick Sort, and Heap Sort, providing a hands-on comparison of their methodologies and performance characteristics.

-   `Sorting_modified.cpp`: Explore modified versions or specific sorting algorithms that address particular constraints or offer unique performance characteristics, such as specialized sorting for nearly sorted arrays or distribution sorts.

-   `ST_RMQ.cpp`: Solve the **Range Minimum Query (RMQ)** problem using a **Sparse Table**. This data structure preprocesses an array to answer RMQ queries in constant time, making it incredibly fast for static arrays.

-   `fenwickTree.cpp`: Leverage the power of the **Fenwick Tree**, also known as a **Binary Indexed Tree (BIT)**. This efficient data structure supports point updates and prefix sum queries in logarithmic time, making it invaluable for competitive programming and various data analysis tasks.

---

### Class Implementations

This section features the foundational building blocks of more complex programs: various data structures implemented as C++ classes. These provide robust and reusable components for larger projects, emphasizing proper encapsulation and efficient operations.

-   `AVLTree.cpp`: A robust implementation of an **AVL Tree**, a self-balancing binary search tree. AVL trees ensure logarithmic time complexity for search, insertion, and deletion operations by maintaining a strict balance factor, making them highly efficient for dynamic datasets.

-   `BST.cpp`: A fundamental implementation of a **Binary Search Tree (BST)**. This class provides the basic operations for searching, inserting, and deleting nodes while maintaining the BST property, serving as a cornerstone for more complex tree structures.

-   `BinomialHeap.cpp`: Dive into the world of priority queues with the **Binomial Heap**. This collection of binomial trees supports efficient merging of two heaps, making it suitable for applications requiring frequent merge operations, such as certain graph algorithms.

-   `Exceptions.hpp`: Define custom **exception classes** for robust error handling. This header file contains specialized exception types that allow for more precise error reporting and graceful recovery in various data structure operations.

-   `LeftistHeap.cpp`: Explore the **Leftist Heap**, another type of mergeable priority queue. Its characteristic "leftist" property ensures efficient merging, making it a valuable alternative to binomial or Fibonacci heaps in specific scenarios.

-   `PriorityQueue.cpp`: A generic implementation of a **Priority Queue** data structure. This class allows elements to be retrieved based on their priority, typically implemented using a heap, essential for tasks like scheduling and graph algorithms (e.g., Dijkstra's).

-   `Queue.cpp`: A basic implementation of a **Queue** data structure, following the First-In, First-Out (FIFO) principle. This class provides fundamental enqueue and dequeue operations, crucial for task scheduling, BFS, and buffer management.

-   `RBT.cpp`: Implement the **Red-Black Tree (RBT)**, another self-balancing binary search tree. RBTs maintain balance through a set of color properties, guaranteeing logarithmic time complexity for all major operations and offering a strong alternative to AVL trees.

-   `Set.cpp`: A fundamental implementation of a **Set** data structure. This class stores unique elements and provides efficient operations for insertion, deletion, and membership testing, typically backed by a hash table or a balanced binary search tree.

-   `SkewHeap.cpp`: Investigate the **Skew Heap**, a self-adjusting heap structure that, while not strictly balanced, performs well on average due to its merging strategy. It offers simple and efficient merge operations, making it a flexible choice for priority queue needs.

-   `String.cpp`: A custom **String class implementation**, providing insights into how string manipulation and memory management can be handled at a lower level, including operations like concatenation, substring extraction, and comparison.

-   `Tree.cpp`: A generic **Tree data structure implementation**, providing the foundational structure for various tree types. This class might include basic node structures and common tree traversal methods.

-   `Vector.hpp`: A custom **Vector (dynamic array) class implementation**. This header file provides a resizeable array similar to `std::vector`, demonstrating dynamic memory allocation, element access, and resizing strategies.

-   `algorithm.hpp`: A header file brimming with **various utility algorithms** and common functions that complement the data structure implementations, such as min/max operations, swap functions, and basic mathematical helpers.

-   `close_Hash_Table.cpp`: An implementation of a **Closed Hashing (Open Addressing) Hash Table**. This approach resolves collisions by probing for the next available slot directly within the hash table's array, demonstrating linear probing, quadratic probing, or double hashing.

-   `disjointSet.cpp`: Master the **Disjoint Set Union (DSU)** data structure. This efficient structure manages a collection of disjoint sets, supporting operations like finding the representative of a set and merging two sets, indispensable for algorithms like Kruskal's and connectivity problems.

-   `graph.cpp`: A generic **Graph data structure implementation**, providing the framework for representing graphs, including functionalities for adding vertices and edges, suitable for both directed and undirected graphs.

-   `heap.cpp`: A basic **Heap data structure implementation**, typically a binary heap. This file demonstrates the core operations of a heap, such as insertion, extraction of the minimum/maximum element, and heapify, fundamental for priority queues and heap sort.

-   `linked_hashmap.hpp`: A header file for a **Linked Hash Map** implementation. This data structure combines the benefits of a hash map (fast lookups) with a linked list (maintaining insertion order), providing predictable iteration order.

-   `list.hpp`: A header file for a generic **List** data structure, potentially implementing either a singly or doubly linked list. This provides an alternative to arrays for dynamic collections where frequent insertions and deletions are required.

-   `map.hpp`: A header file for a generic **Map** (key-value pair) implementation. This abstract class or interface lays the groundwork for various map types, such as hash maps or tree maps, defining fundamental operations like insertion, lookup, and deletion based on keys.

-   `open_Hash_Table.cpp`: An implementation of an **Open Hashing (Separate Chaining) Hash Table**. This collision resolution strategy uses linked lists (or other data structures) at each hash table "bucket" to store elements that hash to the same index.

-   `simple_graph.cpp`: A more basic or simplified **Graph implementation**, perhaps focusing on a specific type of graph (e.g., adjacency matrix for dense graphs) or a subset of graph operations, suitable for introductory examples.

-   `splay_tree.cpp`: Dive into the **Splay Tree**, a self-adjusting binary search tree. Splay trees move frequently accessed nodes closer to the root, improving performance for sequences of operations, though individual operations can take $O(log\ n)$ amortized time.

-   `utility.hpp`: A versatile header file containing **general utility functions** that support various data structure implementations, such as debugging macros, type traits, or common mathematical helper functions.

> `vector.hpp`, `list.hpp`, `priority_queue.hpp`, `linked_hashmap.hpp` and `map.hpp` are the final **assignments** for ST-Lite.

---

### Specific Usage

This section moves beyond generic implementations to showcase practical applications and problem-solving techniques using the data structures and algorithms defined in the other sections. These files demonstrate how the theoretical concepts come to life in real-world scenarios.

-   `Bracket_matching.cpp`: An example of using data structures, typically a **stack**, to solve the **bracket matching** problem. This demonstrates how a stack can be used to validate the correct nesting and pairing of parentheses, braces, and brackets in expressions.

-   `Bucket_usage.cpp`: Using **Bucket** for searching key values in constant time complexity.

-   `Calculator2.cpp`: A more advanced **calculator** implementation. This might involve parsing complex expressions, handling operator precedence, and supporting a wider range of mathematical functions, often leveraging stacks and expression trees.

-   `Caluculator.cpp`: A basic **calculator** implementation. This file demonstrates the fundamental logic for evaluating simple arithmetic expressions, typically using a combination of stacks for numbers and operators.

-   `HfTree.cpp`: An implementation or usage example of a **Huffman Tree** for data compression. This file showcases how Huffman coding can be used to build variable-length codes for characters based on their frequencies, leading to efficient lossless compression.

-   `Memory.cpp`: Code related to **memory management** or specific memory-intensive operations. This might include examples of custom allocators, memory pooling, or techniques for optimizing memory usage in data structures.

-   `Puzzle_Creation.cpp`: Algorithms for **puzzle creation** via disjointset.

-   `Simulator_pq.cpp`: A simulation or example demonstrating the usage of a **priority queue**. This file might model scenarios like task scheduling, event management, or discrete event simulation, where elements need to be processed based on their priority.

-   `Usage_for_DST.cpp`: Demo Usage for **Dynamic Search Table**, including `map`, `set`, `unordered_set` and `unordered_map`.

-   `longlongInt.cpp`: An implementation or usage of **long long integer arithmetic** for handling arbitrarily large numbers. 


## Contributions

Several Codes in this repo still exist bugs.🤡🤡🤡 **PR Welcome**!

## References

- 《数据结构：思想与实现》 翁慧玉，俞勇编著




