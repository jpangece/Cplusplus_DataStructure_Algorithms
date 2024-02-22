#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include "BinaryTree.h"
#include "NodeInfo.h"
using namespace std;


void compress(string fileName)
{
    int nodeCount = 0;
    BinaryTree tree(nodeCount++);
    
    string str;
    
    ifstream fin(fileName);
    fin >> str;
    
    int i = 0, j;
    bool flag = true;
    while (i < (int)str.size())
    {
        Node *node;
        for (j = 0; i + j - 1 < (int)str.size(); j++)
        {
            node = tree.visitThroughPath(str.substr(i, j));
            if (node == nullptr)
                break;
        }
        
        if (node != nullptr)
        {
            cout << NodeInfo(node->getVal(), '@');
            flag = false;
            break;
        }
        
        Node *prev_node = tree.visitThroughPath(str.substr(i, j - 1));
        if (str[i + j - 1] == '0')
        {
            cout << NodeInfo(prev_node->getVal(), '0');
            prev_node->setLeft(nodeCount++);
        }
        else
        {
            cout << NodeInfo(prev_node->getVal(), '1');
            prev_node->setRight(nodeCount++);
        }
        i += j;
    }
    
    if (flag)
        cout << NodeInfo(0, '@');
    
    cout << endl;
}

void decompress(string fileName)
{
    int nodeCount = 0;
    BinaryTree tree(nodeCount++);
    
    NodeInfo node;
    
    
    vector<NodeInfo> save;
    
    ifstream fin(fileName);
    while (fin >> node) 
    {
        save.push_back(node);
        auto [idx, dir] = node;
        
        Node *node = tree.find(idx);
        if (dir == '0')
            node->setLeft(nodeCount++);
        else if (dir == '1')
            node->setRight(nodeCount++);
        else 
            continue;
    }
    
    
    for (auto [idx, dir] : save)
    {
        Node *node = tree.find(idx);
        if (dir == '0')
            node = node->getLeft();
        else if (dir == '1')
            node = node->getRight();
        cout << tree.findPath(node->getVal());
    }
    
    cout << endl;
}


int main(int argc, char *argv[]){
    // TODO: implement your dbc program here!
    if (string(argv[1]) == "-x")
    {
        compress(string(argv[2]));
    }
    else
    {
        decompress(string(argv[2]));
    }
    return 0;
}
