#include "BinaryTree.h"
#include <iostream>
#include <vector>
#define MAX(x, y) ((x) > (y) ? (x) : (y))

using namespace std;

/* ================================== Node =================================== */


Node::Node(const int &val, Node *left, Node *right)
    : val(val), left(left), right(right)
{
}

int Node::getVal() const
{
    return this->val;
}

void Node::setVal(const int &newVal)
{
    this->val = newVal;
}

Node *Node::getLeft() const
{
    return this->left;
}

void Node::setLeft(const int &newVal)
{
    if (this->left)
        this->left->setVal(newVal);
    else 
        this->left = new Node(newVal);
}

Node *Node::getRight() const
{
    return this->right;
}

void Node::setRight(const int &newVal)
{
    if (this->right)
        this->right->setVal(newVal);
    else 
        this->right = new Node(newVal);
}


/* =============================== Binary Tree =============================== */

BinaryTree::BinaryTree(const int &rootValue)
{
    root = new Node(rootValue);
}

BinaryTree::BinaryTree(Node *node) : root(node) {}

BinaryTree::BinaryTree(const BinaryTree &tree) 
{
    auto init = [&](auto &self, Node *thisNode, Node *anoNode) -> void 
    {
        if (anoNode->getLeft()) 
        {
            thisNode->setLeft(anoNode->getLeft()->getVal());
            self(self, thisNode->getLeft(), anoNode->getLeft());
        }
        
        if (anoNode->getRight()) 
        {
            thisNode->setRight(anoNode->getRight()->getVal());
            self(self, thisNode->getRight(), anoNode->getRight());
        }
    };
    
    this->root = new Node(tree.root->getVal());
    init(init, this->root, tree.root);
}

BinaryTree::BinaryTree(std::vector<nodeValue> &source)
{
    root = createFromVariant(source, 0);
}

BinaryTree::~BinaryTree()
{
    auto clear = [&](auto &self, Node *node) -> void 
    {
        if (node == nullptr)
            return;
            
        self(self, node->getLeft());
        self(self, node->getRight());
        delete node;
    };
    
    clear(clear, root);
}

bool BinaryTree::empty() const 
{
    return root == nullptr;
}

Node *BinaryTree::find(const int &key) const
{
    auto _find = [&](auto &self, Node *cur, const int &key) -> Node *
    {
        if (cur == nullptr)
            return nullptr;
        
        if (cur->getVal() == key)
            return cur;
        
        Node *left = self(self, cur->getLeft(), key);
        Node *right = self(self, cur->getRight(), key);
        
        return (left ? left : right);
    };
    
    
    return _find(_find, root, key);
}

std::string BinaryTree::findPath(const int &value) const
{
    vector<char> pathVector;
    
    auto _findPath = [&](auto &self, Node *cur, const int &value) -> std::string 
    {
        if (cur == nullptr)
            return "";
        
        std::string returnString = "";
        
        if (cur->getVal() == value) {
            for (char c : pathVector) 
                returnString += c;
            return returnString;
        }
        
        for (char c : {'0', '1'}) {
            pathVector.push_back(c);
            returnString += self(self, c == '0' ? cur->getLeft() : cur->getRight(), value);
            pathVector.pop_back();
        }
        return returnString;
    };
    
    return _findPath(_findPath, root, value);
}

Node *BinaryTree::visitThroughPath(const std::string &path) const
{
    auto _visitThroughPath = [&](auto &self, Node *cur, int path_idx, const std::string &path) -> Node * 
    {
        if (path_idx == (int) path.size())
            return cur; 
    
        if (cur == nullptr)
            return nullptr;
        
        return self(self, path[path_idx] == '0' ? cur->getLeft() : cur->getRight(), path_idx + 1, path);
    };
    
    return _visitThroughPath(_visitThroughPath, root, 0, path);
}

int BinaryTree::sum() const
{
    auto _sum = [&](auto &self, Node *cur) -> int 
    {
        if (cur == nullptr)
            return 0;
        
        return cur->getVal() + self(self, cur->getLeft()) + self(self, cur->getRight());
    };
    
    return _sum(_sum, root);
}

int BinaryTree::height() const
{
    auto _height = [&](auto &self, Node *cur) -> int
    {
        if (cur == nullptr)
            return 0;
        
        return MAX(self(self, cur->getLeft()), self(self, cur->getRight())) + 1;
    };
    
    return _height(_height, root);
}

void BinaryTree::preOrder() const
{
    auto _preOrder = [&](auto &self, Node *cur) -> void
    {
        if (cur == nullptr)
            return;
        std::cout << cur->getVal() << " ";
        self(self, cur->getLeft());
        self(self, cur->getRight());
    };
    
    _preOrder(_preOrder, root);
    cout << endl;
}

void BinaryTree::inOrder() const
{
    auto _inOrder = [&](auto &self, Node *cur) -> void
    {
        if (cur == nullptr)
            return;
        self(self, cur->getLeft());
        std::cout << cur->getVal() << " ";
        self(self, cur->getRight());
    };
    
    _inOrder(_inOrder, root);
    cout << endl;
}

void BinaryTree::postOrder() const
{
    auto _postOrder = [&](auto &self, Node *cur) -> void
    {
        if (cur == nullptr)
            return;
        self(self, cur->getLeft());
        self(self, cur->getRight());
        std::cout << cur->getVal() << " ";
    };
    
    _postOrder(_postOrder, root);
    cout << endl;
}

bool BinaryTree::allPathSumGreater(const int &sum) const
{
    vector<int> vec;
    auto _allPathSumGreater = [&](auto &self, Node *node, const int &sum) -> bool 
    {
        if (node == nullptr)
            return true;
        
        if (!node->getLeft() && !node->getRight())
        {
            int pathSum = node->getVal();
            for (int x : vec)
                pathSum += x;
            if (pathSum > sum)
                return true;
            else 
                return false;
        }

        vec.push_back(node->getVal());
        bool flag = self(self, node->getLeft(), sum) && self(self, node->getRight(), sum);
        vec.pop_back();
        
        return flag;
    };
    
    return _allPathSumGreater(_allPathSumGreater, this->root, sum);
}

bool BinaryTree::operator<(const BinaryTree &tree) const
{
    auto covered = [&](auto &self, Node *thisNode, Node *anoNode) -> bool 
    {
        if (thisNode == nullptr)
            return true;
        
        if (anoNode == nullptr)
            return false;
        
        if (thisNode->getVal() == anoNode->getVal())
            return (
                self(self, thisNode->getLeft(), anoNode->getLeft()) &&
                self(self, thisNode->getRight(), anoNode->getRight())
            );
        
        return false;
    };
    
    return covered(covered, this->root, tree.root);
}

bool BinaryTree::operator<<(const BinaryTree &tree) const
{
    auto covered = [&](auto &self, Node *thisNode, Node *anoNode) -> bool 
    {
        if (thisNode == nullptr)
            return true;
        
        if (anoNode == nullptr)
            return false;
        
        if (thisNode->getVal() == anoNode->getVal())
            return (
                self(self, thisNode->getLeft(), anoNode->getLeft()) &&
                self(self, thisNode->getRight(), anoNode->getRight())
            );
        
        return false;
    };
    
    auto contained = [&](auto &self, Node *node) -> bool
    {
        if (this->root == nullptr)
            return true;
        
        if (node == nullptr)
            return false;
        
        if (covered(covered, this->root, node))
            return true;
        
        return self(self, node->getLeft()) || self(self, node->getRight());
    };
    
    return contained(contained, tree.root);
}

BinaryTree &BinaryTree::operator=(const BinaryTree &tree)
{
    if (tree.empty())
    {
        this->root = nullptr;
        return (*this);
    }
    
    this->root = new Node(tree.root->getVal());
    auto copyTree = [&](auto &self, Node *thisNode, Node *anoNode) -> void 
    {
        if (anoNode->getLeft())
        {
            thisNode->setLeft(anoNode->getLeft()->getVal());
            self(self, thisNode->getLeft(), anoNode->getLeft());
        }
        if (anoNode->getRight())
        {
            thisNode->setRight(anoNode->getRight()->getVal());
            self(self, thisNode->getRight(), anoNode->getRight());
        }
    };
    
    copyTree(copyTree, this->root, tree.root);
    return (*this);
}

Node *BinaryTree::createFromVariant(const vector<nodeValue> &source, const int &rootIndex) 
{
    if (rootIndex >= (int) source.size())
        return nullptr;
    auto rootValue = get_if<int>(&source[rootIndex]);
    if (!rootValue)
        return nullptr;
    int rightIndex = 2 * (rootIndex + 1);
    int leftIndex = rightIndex - 1;
    return new Node(*rootValue, createFromVariant(source, leftIndex), createFromVariant(source, rightIndex));
}

