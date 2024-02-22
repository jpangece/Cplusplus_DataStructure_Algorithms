#include "node.h"

Node::Node(int _value, int _n) {
    value = _value;
    n = _n;
    child_num = 0;
    height = 0;
    parent = NULL;
    children = new Node*[_n];
}

Node::~Node() {
    int i=0;
    for (i=0; i<child_num; i++) {
        delete children[i];
    }
    // parent = NULL;
    delete[ ] children;
}

void Node::addChild(Node *child) {
    if (child_num == n) {
        delete(child);
        throw tooManyChildren();
        return;
    }

    children[child_num] = child;
    child_num++;
    child->parent = this;

    if (height < 1) {
        height++;
    }

    Node* parent_ptr = parent;
    int parentHeight = height+1;

    while(parent_ptr != NULL) {
        if (parent_ptr->height < parentHeight) {
            parent_ptr->height = parentHeight;
        }
        parentHeight++;
        parent_ptr = parent_ptr->parent;
    }
}

void Node::addChild(int _value) {
    if (child_num == n) {
        delete(children[child_num]);
        throw tooManyChildren();
        return;
    }

    // addChild(child);
    // child = NULL;

    children[child_num] = new Node(_value, n);
    children[child_num]->parent = this;
    child_num++;

    if (height < 1) {
        height++;
    }

    Node* parent_ptr = parent;
    int parentHeight = height+1;

    while(parent_ptr != NULL) {
        if (parent_ptr->height < parentHeight) {
            parent_ptr->height = parentHeight;
        }
        parentHeight++;
        parent_ptr = parent_ptr->parent;
    }
}

void Node::traverse(vector<int>& traverseValue) {
    int i=0;
    traverseValue.push_back(value);
    for (i=0; i<child_num; i++) {
        children[i]->traverse(traverseValue);
    }
}

bool Node::contain(Node *sub) {
    vector<int> traverseValue;
    int i=0;
    traverse(traverseValue);
    for (i=0; i<(int)traverseValue.size(); i++) {
        if (sub->value == traverseValue[i]) {
            return true;
        } 
    }
    return false;   
}

int Node::getHeight() {
    return height;
}

Node& Node::operator[](int i) {
    if (i >= child_num) {
        throw invalidIndex();
    }
    return *children[i];
}
