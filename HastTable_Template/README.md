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

Here _bucketIt_ means which bucket the iterator is in, _listItBefore_ means the iterator pointer that points “before” the key-value pair in the linked list.

We’ll give a simple explanation of the “before” iterator. If you want to erase a node in a linked list, you need to link the previous node and the next node. However, the list is single directional, so that you can’t get the previous node in O time unless you’ve saved it. If you always use a “before” iterator, the deletion of node will be possible, and you can access the current node by “next” of the “before” iterator. This is a built-in functionality of the std::forward_list class. The class also provides a before_begin method, which is different from other STL containers. You can check the documentation for more information. 


Since the list is single directional (in order to save memory), the iterator only supports single directional iteration. The operator ++ is already overloaded for you. The iterator doesn’t have a const version, you can implement it if you’reinterested in iterators, but it’s not mandatory. You can see the detail implementation of the iterator in the starter code. 

We also defines a variable

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/da63f6c9-1813-4d55-8e8b-12f5188bf075)

in the hash table. It provides an O(1) access to the the first key-value pair in the hash table. You need to update its value whenever an insert, erase or rehash operation is applied to the hash table.

What’s more, the hash table supports basic iteration and range-for loops with the help of iterators and two methods:
begin and end. More specifically, with the C++98 standard you can iterate the hash table by:

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/482bc14f-fc12-4f5b-ba77-115602fb07c5)

With C++11, you can use the following as an alternative:

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/bb92fc57-4619-4f50-9b8b-ee85a69b6fa1)

With C++17, you can even do it in a more graceful way (similar to python and some other modern languages):

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/1afe39e5-ded0-40ab-a5ce-d510550cf8ee)

The order of the output of the above code is arbitrary and is dependent on implementation. We’ll not test the internal order of the key-value pairs in your hash table (returned by iteration), since they’re meant to be “unordered”.

### 2.2 Comparison of Hash Table and Linked List
We also recommend you to study the performances of hash table and linked list. Basically, you will compare these classes:

• HashTable
• std::unordered_map
• std::list or std::forward_list (their performances should be similar)

You can design your own comparison metric. For example, you can insert n key-value pairs into all these classes, then run k find, update, insert or erase operations. In a linked list, you can use linear search, it should be very simple to implement these operations with std::list or std::forward_list.

No report is required, but you are recommended to study their differences for your better understanding in terms of hash table.

#### 2.2.1 Hints

• The performance of programs on Windows is usually not stable, so you should do the experiments on a Unix based system.
• You may want to write another program to do this study.
• You can use the C++11 pseudo-random number generation library to generate more randomized numbers (instead of using std::rand).
• You can use the C++11 chrono library to get more accurate runtime of functions than std::clock.
• (Optional) You can use GNU Perf (only available on Linux) to find the bottleneck of your implementation.
• Although the major factor that affects the runtime is the size of the input array, however, the runtime for an algorithm may also weakly depend on the detailed input array. Thus, for each size, you should generate a number of arrays of that size and obtain the mean runtime on all these arrays. Also, for fair comparison, the same set of arrays should be applied to all the data structures.
• You should try at least 5 representative sizes

### 3. Implementation Requirements and Restrictions
#### 3.1 Requirements
• You must make sure that your code compiles successfully on a Linux operating system with g++ and the options -std=c++1z -Wconversion -Wall -Werror -Wextra -pedantic.
• You should not change the definitions of the functions and variables, as well as the public or protected keywords for them in hashtable.hpp.
• You can define helper functions and new variables, don’t forget to mark them as private or protected.
• You should only hand in one file hashtable.hpp.
• You can use any header file defined in the C++17 standard. You can use cppreference as a reference.

You only need to implement the methods (functions) marked with “TODO” in the file hashtable.hpp. Here is a list of the methods (functions):

• Copy Constructor and Assignment Constructor
• findMinimumBucketSize
• find
• insert
• erase
• operator[]
• rehash

#### 3.2 Memory Leak
Hint: You’re not going to use any dynamic memory allocation functions (new, malloc, etc.) directly in this project, thus it’s not possible to have memory leak in your program. This section is only for your reference.

You may not leak memory in any way. To help you see if you are leaking memory, you may wish to call valgrind, which can tell whether you have any memory leaks. (You need to install valgrind first if your system does not have this program.) The command to check memory leak is:

valgrind --leak-check=full <COMMAND>

You should replace <COMMAND> with the actual command you use to issue the program under testing. For example, if you want to check whether running program

./main < input.txt

causes memory leak, then <COMMAND> should be “./main < input.txt”. Thus, the command will be 

valgrind --leak-check=full ./main < input.txt

### 4. Acknowledgment
The programming is co-authored by Yihao Liu, an alumni of JI and the chief architect of JOJ.

#### References
[1] std::unordered_map - cppreference : https://en.cppreference.com/w/cpp/container/unordered_map

[2] std::forward_list - cppreference : https://en.cppreference.com/w/cpp/container/forward_list

[3] Range-based for loop - cppreference: https://en.cppreference.com/w/cpp/language/range-for

### Appendix
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/a197f76e-1dca-4fd5-bedf-3a5e603077e1)
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/26fb7fa0-67be-4806-94f0-f2a5538366d2)

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/6d42a6f2-77d9-4129-8f60-815983b4aae1)
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/0079b2f5-cc3a-4082-a180-753903c16f82)
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/d18f6d8c-0941-4c41-9aa1-db90cbff35f9)
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/198c7c24-10ef-4847-b7ca-3055440451c7)
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/1cc0aa9e-5d04-4285-ada8-195185c21334)
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/d3069481-9698-44f4-b633-ff04019dbaa4)
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/15387a89-c338-4d17-a07a-1cefa7c36ec3)
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/2672330d-c2d1-4e11-9a7c-6c1c2fa5c96f)
