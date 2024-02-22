## 1 Introduction
Implementation of a STL-like hash table data structure.
In the Standard Template Library, hash table is implemented as std::unordered_map, which is introduced in the C++11 standard. It is an associative container that contains key-value pairs with unique keys. Search, insertion, and removal of elements have average constant-time complexity. It is named “unordered” because there already exists an ordered data structure of key-value pairs based on RB-tree, std::map.

Internally, the elements are not sorted in any particular order, but organized into buckets. Which bucket an element is placed into depends entirely on the hash of its key. This allows fast access to individual elements, since once the hash
is computed, it refers to the exact bucket the element is placed into.
The C++ standard does not mandates whether unordered_map use separate chaining or open addressing for collision resolution. Actually, different compilers use different policies: g++ uses separate chaining and LLVM/Clang uses open
addressing. In this project, you’re going to use separate chaining with std::forward_list, which is easier to implement and maintain.

You will also have a basic knowledge on iterators in C++: what are iterators and how they work. You can further try to use range-based for loops (a feature introduced in C++11) to make use of iterators.

## 2 Programming Tasks
### 2.1 The HashTable Template
#### 2.1.1 Overview
In the starter code at the very bottom, the template class is defined:
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/9778c847-d7c2-484c-b644-e7b38e7c8cd3)

Here Key and Value are the types of the key-value pair stored in the hash table. Hash is the type of a function object, whose instance returns the hash value of a key. The standard library defines a class template std::hash<T>, which can be used as the default. Similarly, KeyEqual is another type of a function object, whose instance returns whether two Key objects are equal. It is used to determine which key-value pair should be returned when there is a hash collision, and it also help ensure the keys are unique in the hash table.
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/9169c4af-6c35-4107-b51b-50e8225c163c)
#### 2.1.2 Number of Buckets
Basically, you will use the “Hashing by Modulo” scheme. Suppose n is the number of buckets in the hash table, you should put a key into the i-th bucket where i = hash(key) mod n. You should choose the hash table size n as a large prime number in ideal.

The number of buckets in your hash table should be dynamic (like a std::vector, but not exactly the same). A vector in C++ doubles its size when it is full (in most implementations), but this strategy may not be applied to a hash
table because you should choose a prime number as the new size. Here we define the following strategy to choose a new hash table size before a rehash.

• First we define a list of prime numbers, each doubles its predecessor approximately. The list is taken from the g++ source code, and is provided with you as the file hash_prime.hpp.

• When a hash table is considered full (its load factor exceeds a preset maximum value), use the next prime as the new number of buckets and perform a rehash.

Furthermore, the maximum load factor value can also be changed in runtime; you can set a minimum number of buckets manually (i.e., to prevent rehashes if you’ve already know the data size) despite the load factor is small. **One important thing is that whenever the number of buckets is changed, you need to do a rehash**. You will fulfill all these requirements in the function findMinimumBucketSize. Please read its description carefully before implementing it. Though not mandatory, you are encouraged to use binary search to find the nearest prime. You can use the standard function std::lower_bound to perform the binary search so that you don’t need to implement it by yourself
#### 2.1.3 Internal Data Structures
We’ve already defined the internal data structures for you in this project. The HashNode is a key-value pair. The HashNodeList is a single directional linked list in each bucket, here we use std::forward_list to implement it. The HashTableData is a std::vector of these lists, representing the buckets in the hash table.
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/33cb01cb-3456-4d75-a603-bd15098e58ae)
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/4dce1fcc-e76c-407c-a747-3a484b832398)

tableSize means the number of key-value pairs in the hash table. You can also add your own private / protected variables (or attributes) after these if necessary, but mostly the defined ones are enough. Here we use the protected keyword so that your HashTable class can be further inherited and overwritten for testing.

#### 2.1.4 Iterators
Iterators will be introduced.  First of all, why you need to use iterators in this project? Supposing you want to find a key-value pair in the hash table, and then you may erase it from the hash table if it satisfies certain conditions. There are three implementations to achieved this:

(1) Use the find method to find a key-value pair, use the erase method to erase it.
(2) Use the find method to find a pointer to a key-value pair, use the erase method which accepts a pointer to erase it.
(3) Use the find method to find an iterator to a key-value pair, use the erase method which accepts an iterator to erase it.
In the first implementation, two lookups of the key is performed.

In the second implementation, only one lookup of the key is performed. 

However, the user can access the internal data structure of the hash table with the pointer, which is dangerous and violates the rule of packaging in Object-Oriented Programming.

In the third implementation, only one lookup of the key is performed as well, and the iterator only provides a restricted access to the key-value pair, so it’s a better solution. 

In this project, we’ve already provided you with a completed iterator. You are going to use the iterator in the implementation of other methods in the hash table.

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/b6db0895-1b93-424d-be3e-73b56e51baa5)



