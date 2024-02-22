## 1 Introdcution
In this project, we are going to implement an algorithm to find the shortest path in a given directed graph. The detailed data structure and algorithmis up to our choice. We will gain experiences about choosing and designing algorithms under required specifications from this project.

## Programming
### 2.1 Input and Output
We are expected to read input from <stdin> and print output through <stdout>. The input will provide us a graph, as well as subsequent queries which are pairs of sources and destinations. Our output should provide the shortest distance of each query.

#### 2.1.1 Input Specification
The first line in the input specifies the number of nodes |V| in the graph G = (V, E), where the nodes in the graph are indexed from 0 to |V| − 1. The second line specifies the number of edges, assumed as E, in the graph. Each of the subsequent E lines represents a **directed** edge by 3 numbers in the form:

<start_node> <end_node> <weight>

where both <start_node> and <end_node> are integers in the range [0, |V| − 1], representing the start node and the end node of the edge, respectively, and <weight> is an **integer** representing the edge weight. Here we can assume that there will be no parallel edges in this input sequence.

After E lines of edges, the program will be fed with a series of queries. Each query line represents a pair of source and destination nodes that we want to find shortest path from the source node to the destination node, in the form:

<source_node> <destination_node>

The graph specified in our input format is a **directed graph with integer edge weights**. Notice that the edge weights might be negative in some inputs.

Hint: When some edges with negative weights exist in the graph, are there possibilities that the shortest path between two nodes do not exist, i.e. will there exist a situation where the source and the destination nodes are connected, but **the shortest path between connected nodes becomes infinite?**

We define a graph to be invalid if there exists a pair of source and destination nodes (not necessarily the input source-destination pairs) that are connected but the shortest path is not finite. The graph is **valid** otherwise.

#### 2.1.2 Terminate Condition
Oue program should exit when a) the graph is invalid or b) the <source_node> in a query pair is -1. When the graph is invalid, our program is expected to output:

Invalid graph. Exiting.

#### 2.1.3 Exmaple
Here is an example of a valid graph and a few subsequent queries:

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/64106d7b-fd6a-44b4-816c-5cfa341ce6dc)

It represents the following directed graph with 4 pairs of source and destination node:

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/0a9432fe-fdbf-4efe-82c6-24cfa1aec65f)

The input will be provided from the **standard input**.

#### 2.1.4 Output Specification
Our program writes to the **standard output**. Our program should first read the input graph, then respond to each of the input source-destination pairs. During the computation of shortest paths, we might find that the input graph is invalid. For invalid graphs, we should not output any of the calculated shortest path results, but instead print the warning information and exit the program.

a) **Invalid Graph**

If the graph is invalid, our program should print:

Invalid graph. Exiting.

And **exit the program as soon as possible** (Hint: we should have enough knowledge about this before any queries).

b) **Valid Graph**
If the graph is valid, we should respond to each input line of the source-destination pair sequentially in the output. The detailed requirements for each input line are as follows.

If the input graph is valid and the shortest path exists for the input source-destination pair, our program should print the length of the shortest path. Specifically, it should print the following line for each query:

<length>
  
where <length> specifies the length of the shortest path. In the special case where the source node is the same asthe destination node, the shortest path length is 0.

When the source is not connected with the destination (when no path exists), the program should output the text

INF.

The output for previous example should be:

![image](https://github.com/jpangece/Cplusplus_DataStructure_Algorithms/assets/122253772/ed0cceaf-1d55-4f40-b4c8-798a4452586e)

For more information related to the input and output of the whole program, we may refer to the provided main.cpp, which is identical to what we will use to test our program. Do not include this main.cpp in we submission file.

### 2.2 Methods to Implement
In this project, we are asked to implement our algorithm with two methods: readGraph() and distance(). However, we are free to add internal helper functions up to our choice.

#### 2.2.1 Graph Reading
In the method readGraph(), we should take all the inputs related to the graph and store it in the data structure we choose. Initialization of the graph should be done within this method. Notice that the queries are not read here.

#### 2.2.2 Shortest Path Finding
In the method distance(), we should output the length of the shortest path found for the current query in the form
specified in 2.1.2.

### 2.3 Data Structure and Algorithm
In this project, we do not explicitly ask to use specific data structures or algorithms. We are free to modify the definition of the Graph class in shortestP2P.hpp by adding internal data and functions, but do not change the name of the class as well as the methods specified in 2.2.

## 3 Implemetation Requirements and Restrictions
### 3.1 Error Checking
We do not need to do any error checking of the input graph except for its validness and we can assume that all the
inputs are syntactically correct.

### 3.2 Requirements
• We must make sure that our code compiles successfully on a Linux operating system with g++ and the options
-std=c++1z -Wconversion -Wall -Werror -Wextra -pedantic.

• We should only hand in two files shortestP2P.hpp and shortestP2P.cc.

• We are not allowed to include additional libraries, in either shortestP2P.hpp or shortestP2P.cc. Do not
submit main.cpp.

• Output should only be done where it is specified

### 3.3 Memory Leak
We may not leak memory in any way. To help to see if we are leaking memory, we may wish to call valgrind, which can tell whether we have any memory leaks. (We need to install valgrind first if our system does not have this program.) The command to check memory leak is:

valgrind --leak-check=full <COMMAND>

We should replace <COMMAND> with the actual command we use to issue the program under testing. For example, if we want to check whether running program

./main < input.txt

causes memory leak, then <COMMAND> should be “./main < input.txt”. Thus, the command will be

valgrind --leak-check=full ./main < input.txt

## 4 Acknowledgement
This project is jointly designed by Hongyi Xin, Xiaotian Shi, Yidan Xing, Kaiyang Chen and Haifeng Jia






