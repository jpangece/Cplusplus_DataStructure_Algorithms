#include <iostream>
#include <algorithm>
    
template <class T>   
bool Dlist<T>::isEmpty() const
{
    return (first == nullptr);
}

template <class T>
void Dlist<T>::insertFront(T *op)
{
    if (this->isEmpty())
    {
        first = last = new node();
        first->next = first->prev = nullptr;
        first->op = op;
    }
    else 
    {
        node *newNode = new node();
        newNode->next = first;
        newNode->prev = nullptr;
        newNode->op = op;
        
        first->prev = newNode;
        first = newNode;
    }
}

template <class T>
void Dlist<T>::insertBack(T *op)
{
    if (this->isEmpty())
    {
        first = last = new node();
        first->next = first->prev = nullptr;
        first->op = op;
    }
    else 
    {
        node *newNode = new node();
        newNode->prev = last;
        newNode->next = nullptr;
        newNode->op = op;
        
        last->next = newNode;
        last = newNode;
    }
}

template <class T>
T *Dlist<T>::removeFront()
{
    if (this->isEmpty())
        throw EmptyList();
    
    T *ret = first->op;
    
    node *second = first->next;
    
    if (second != nullptr)
        second->prev = nullptr;
    
    delete first;
    first = second;
        
    if (first == nullptr)
        last = nullptr;
        
    return ret;
}

template <class T>
T *Dlist<T>::removeBack()
{
    if (this->isEmpty())
        throw EmptyList();
    
    T *ret = last->op;
    
    node *second = last->prev;
    
    if (second != nullptr)
        second->next = nullptr;
        
    delete last;
    last = second;
        
    if (last == nullptr)
        first = nullptr;
    
    return ret;
}

template <class T>
Dlist<T>::Dlist()
{
    first = last = nullptr;
}

template <class T>
Dlist<T>::Dlist(const Dlist &l) : Dlist()
{
    this->copyAll(l);
}

template <class T>
Dlist<T> &Dlist<T>::operator=(const Dlist &l)
{
    this->removeAll();
    this->copyAll(l);
    return (*this);
}

template <class T>
Dlist<T>::~Dlist()
{
    this->removeAll();
}

template <class T>
void Dlist<T>::removeAll()
{
    while (!this->isEmpty())
    {
        this->removeFront();
    }
}

template <class T>
void Dlist<T>::copyAll(const Dlist &l)
{
    node *cur = l.first;
    while (cur)
    {
        T *op = new T(*cur->op);
        this->insertBack(op);
        cur = cur->next;
    }
}
