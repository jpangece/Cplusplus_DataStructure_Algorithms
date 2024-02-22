## Background

After experiencing Blackjack and The Disappearance of Suzumiya Haruhi, Kyon want to write down these two special experiences. However, the complicated game rule and the jumping timeline really puzzled Kyon. Below is from Kyon's monologue.

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/2342cee9-c192-4af8-bf1d-f265a7fd4ce7)

## Task 1 : Implementation of N-ary Tree
Our first task is to help Kyon implement the class of n-ary tree. A tree ADT organizes and manages data in a hierarchical tree structure. An n-ary tree is a generalization of a binary tree where any node in the tree has exactly one parent, except one node called root node, and any
node may have zero up to n children, i.e. n is the **maximum number of child** for each node. For an example, see the following figure:

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/6a70b185-f1d0-4d2e-8eec-6e49e50bc302)

A tree ADT stores data in each node.

### 1.1 Terminology
#### 1.1.1 Descendants
A descendant of a node X is a child of X or a descendant of a child of X.
For instance, the descendants of 2 in the previous figure are 4, 5, 6, 8, and 9.

#### 1.1.2 Subtree
A subtree of the tree T is a tree consists of a node in T and all of this node's descendants.
For instance, if we name the tree above as T, then the tree rooted in 2 is a subtree of T.

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/4b220fce-8740-4f10-ba8f-88ea213b9756)

#### 1.1.3 Leaf
A node with no child is called a leaf node. For example, node 4, 5, 8, 9 and 7 are leaf nodes.

####  1.1.4 Path
A path is a sequence of nodes such that a next node in the sequence is a child of the previous one.

##### 1.1.5 Height
The height of a node is the length of a longest path from the node to a leaf.
For example, height(1) = 3, height(9) = 0

### 1.2 Implementation
A node of a tree can be represented by the following class and a tree ADT can be represented by the node corresponding to its root.

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/5a6d270f-a591-436a-8966-2d7d1bee78fc)
![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/ae8c38cd-6d5b-4e91-8d98-39fcbb490980)

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/47ea1b70-0e73-4fe7-90d3-f672980b2e50)


## Task 2: Output the Tree of a Story in a Pre-order Way and Form a Story
Not long after Blackjack, in December, Kyon experienced a more outrageous incident: Suzumiya Haruhi disappeared from his life. The graph below briefly introduced the whole story of Kyon in the format of a n-ary tree.

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/739fd8f4-6ba4-4aa7-9e0e-5d81c2c2578d)

Each number in the bracket is the index of the string vector storyProcess . Our task is to build a tree like this. The shape should be the same as above and the value in each node should be the same as the number in the bracket, for example, the value of the root node should be 0. Traverse it in a pre-order way, get the index sequence of the story and output the string vector storyProcess according to the index sequence we get. The index sequence for output is exactly the same as the pre-order traverse of the tree we build. For example, the pre-order traverse of the below tree is 6 8 9, then we output storyProcess according to the sequence storyProcess[6] , storyProcess[8] , storyProcess[9] . We do not need to add a blank space or a line break between each storyProcess[i] , but we need to add a line break at last.

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/76f7fe68-dae0-4443-b992-22d92620a0c9)

The string vector storyProcess is given in constant.h. For this task,we should put our main function in the file storyTeller.cpp, compress the file storyTeller.cpp and node.cpp into one zip/tar file. 

Remember to check memory leak by valgrind.
node.h and constant.h files will be provided in the Starter file.











