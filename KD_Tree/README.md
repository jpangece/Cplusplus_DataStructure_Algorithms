## 1 Introduction
implementing a STL-like K-D Tree (K-Dimensional Tree) data structure. 
Though there is no similar data structure in STL, you will follow the same type of APIs in other container.

The K-D Tree will be able to handle basic operations: initialization, insertion, deletion and finding minimum / maximum
node on a dimension. There are also some advanced features of a K-D Tree, such as range search and nearest neighbor
search, which are very useful in data analysis of high dimensional data. Luckily, they are not required in this project
due to the hardness of the implementation.

Similar to the hash table ahead, key-value pairs are saved in the K-D Tree. The key is a high-dimensional data point, represented by std::tuple, in which the data type on each dimension can be customized. The value type is a template parameter of any data type, which does not affect the behavior of the K-D Tree.

You will continue to study the STL-like iterators in this project. You need to implement iterators in a tree structure.
It is similar to the iterator in std::map or std::set (based on RB-tree).

## 2 Programming
### 2.1 The KTRee Template
#### 2.1.1 Overview
Template class is defined as below:

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/ab63bedf-33a3-4056-916d-482bf1e9d3fd)

Note that parameter pack (...) is widely used in this project, it is a feature in the C++11 standard for more flexible template programming. Basically it means a pack of any number of template parameters (can be 0, 1 or more). Usually if the “...” is before a parameter name, it is a parameter pack; if the “...” is after a parameter name, it is an unpacking of the parameter pack.

At first we define an abstract template class with only one parameter pack as template argument, and the name of the parameter pack is omitted since we don’t need it. Then we define a partial template specialization of the abstract template, so that we only accept <std::tuple<KeyTypes...>, ValueType> as template parameter. If other types of template parameter are provided, the compiler will fall back to the abstract template base and throw an compile error since **class** **KDTree** is an incomplete type.

For example, if you want a 2D Tree, each dimension is an integer and the saved value is also an integer, the actual type of the tree is KDTree<std::tuple<int, int>, int>. 

Here the typedefs, Key and Value, are the types of the key-value pair stored in the K-D Tree. KeySize is the dimension of the key, and we use a static_assert to ensure the dimension is at least one during compilation. You can try to compile KDTree<std::tuple<>, int> and find what happened. 

For ease of your implementation, we assume all data types in Key can be compared by std::less and std::greater, so that you don’t need to write customized comparators for the K-D Tree. 

Don’t be too afraid of these new grammars in C++11, at least we’ve already defined all of the classes and functions for you and you don’t need to write anything related to parameter packs.

#### 2.1.2 Internal Data Structures
Internal Data Structure is already defined as below:

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/5b83c85d-a6d7-451c-bc1f-aaf9fc6ba5af)

The parent of the root node should be nullptr, and the tree only need to save the root node. It’s a very trivial definition, but it should be enough for the whole project.

#### 2.1.3 Interators
In hash table, we use a self-defined iterator containing two STL iterators (of vector and list). Iterators for these linear data structure can be simple implemented: for a vector, you only need to advance the index; for a list, you only need to make it pointing to the next node. However, when iterating a tree, it’s different that you need to follow a certain tree traversal order.

The definition of the iterator is also trivial. You only need to record a pointer to the K-D Tree, and a pointer to the current node.

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/e3db5c06-1250-4213-a2e6-fa3a2796e0fc)

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/281cbdc4-f5fa-4659-8b44-89e3f3353332)

begin and end methods are provided. The begin method finds the left most leaf of the tree, and the end method uses nullptr as the current node.

Your task is to write the increment and the decrement method in the Iterator class. You should use a depth-first in-order traversal of the tree to increment the iterator, which means, when you have a full iteration of the tree and print each node, the order of the output should be the exactly same as an depth-first in-order traversal.

Here’s a detailed explanation about the increment method. When a increment occurs, if the current node has a right subtree, the next node should be the left most leaf in the right subtree; otherwise (if the current node doesn’t have a right subtree), you should move up (by parent pointer) and find the first ancestor node so that the current node is in the left subtree of the ancestor node. When you increment the right most leaf node in the tree, you’ll find that the node is not in any of its ancestors’ left subtree, so you should end the loop and set the next node as nullptr.

The decrement method is a reverse of the increment method, think about how to implement it by yourself. 

The behavior of doing an increment on the end iterator is an undefined behavior. Similarly, doing an decrement on the begin iterator is also an undefined behavior. For ease of debugging, you can throw an error if these operations happened, but we won’t test your code with these cases. Note that doing an decrement on the end iterator is allowed, which will return the right most leaf node.

If all of your implementation is correct, range-for[1] loops will be automatically supported for the K-D Tree. Try this to evaluate your code:

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/c9e9f435-b6d9-4a33-bdbb-d46ac9d905c4)

#### 2.1.4 The Dynamic Methods
There are three “dynamic” methods implemented in the starter code: findMinDynamic, findMaxDynamic and eraseDynamic. They are only for your reference, and you do not need to call them in your implementation. You can try to understand these functions and use them in the testing.

### 2.2. Operations
#### 2.2.1 Initialization
To initialize an empty K-D Tree, you can set the root to nullptr.

To initialize a K-D Tree with another K-D Tree, you should traverse and make a deep copy of all nodes in that tree.

To initialize a K-D Tree with a vector of data points, a trivial idea is to insert the data points one by one, the time complexity is O(kn log n) obviously. However, it is very likely to form a not balanced tree and lead to a poor performance. A better idea is to find the median point of the current dimension so that the data points can be equally partitioned into the left and right subtree.

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/dc17889c-a3b6-4f69-8f80-7f6f49fa3092)

Before inserting the data, you should make sure there is no duplication of key. A simple method is to run a stable sort (std::stable_sort) on the data, and then use std::unique to remove duplicate keys with reverse iteration (so that the latest value of the same key is preserved).

Hint: You can use rbegin and rend for reverse iteration, and get the corresponding forward iterator by it.base().

Recall the linear time selection algorithm, the time complexity of finding the median and partitioning the vector is O(kn), according to the Master theorem, the overall time complexity is also O(kn log n). If there are even number of elements in a vector, use the left one as the median point, this may lead to some unbalance to the tree, but mostly it can be ignored.

Hint: you can use STL functions to efficiently find the median and partition the vector. Check std::nth_element. You may also need the compareKey function to compare tuples on a certain dimension, it is already implemented in the starter code

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/0e666a5d-c1be-4611-ac9b-6cb5f1188456)

**You should use this function for finding the minimum or maximum element in the k d tree and for other funtions like insertion, deletion and etc, you are free to have your own compare function.**

Additionally, you’ll need to implement both the copy constructor and overload the = operator for deep copy. When you are implementing those functions, please think carefully about how you will deal with the original tree.

#### 2.2.2 Insertion and Find Minimum / Maximum
The pseudocode is omitted here because detailed explanations for these operations are already in the lecture slides. Think carefully about what’s the difference of finding minimum and maximum, do not directly copy the code. We’ll briefly explain the template parameter for dimension here. The findMin method has the following definition:

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/8cef617a-c409-45b8-974b-2511ddd2e372)

The first template parameter **DIM_CMP** means the dimension where nodes should be compared on, the second template parameter **DIM** means the dimension of the current node (depth mod k in the tree). We help you define the next dimension **DIM_NEXT** which can be used as template parameter recursively or used in other template methods.

For example, you can use **findMin<DIM_CMP, DIM_NEXT>(node->left)** to recursively find the minimum node in the left subtree on **DIM_CMP**; you can also use **compareNode<DIM_CMP, std::less<> >** to compare the nodes on **DIM_CMP**.

#### 2.2.3 Deletion
The deletion operation is a bit more complex, we’ll also provide the pseudocode for it. Notice that in deletion, we first search for the minimum element in the right subtree, before proceeding to the maximum element in the left subtree.

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/7538ffb6-bc5b-4dc1-815b-a22a66570a97)

Hint: In order to find the minimum / maximum on the current dimension in the left / right subtree, the comparison dimension should be the current dimension, and the starting dimension should be next of the current dimension. Think carefully about how to call the **findMin** and **findMax** method.




## 3 Acknowledgement
The programming is co-authored by Yihao Liu, an alumni of JI and the chief architect of JOJ.

### Reference
[1] Range-based for loop - cppreference: https://en.cppreference.com/w/cpp/language/range-for

### Appendix
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/e2f9e79d-1860-495c-b6a4-38be25609d8f)
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/0fb4845a-91e4-4f10-93bb-f4741bb2eb22)
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/30bb4c2c-72b1-4dc6-a737-3ba9e85b59cc)
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/dc796c44-7d92-48af-b4e8-7f9579b1f760)
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/9cda695d-b7dd-4b18-82e1-a684ccaf5dbe)
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/f2bf1252-87f5-4d77-9afc-0629a44d9868)
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/7c976215-de46-47e0-a445-03478d1a097e)
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/8e8af58d-2417-4e91-bd8f-0a7e8cf1de1a)
