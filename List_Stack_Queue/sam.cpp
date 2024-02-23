#include <algorithm>
#include <iostream>
#include <string>
#include "Dlist.h"
#include "Instr.h"
using namespace std;

class Stack 
{
private:
    Dlist<int> dlist;
    
public:
    ~Stack()
    {
        while (!this->isEmpty())
        {
            this->pop();
        }
    }
    
    int pop()
    {
        int *ptr = dlist.removeBack();
        int val = *ptr;
        delete ptr;
        
        return val;
    }
    
    void push(int val)
    {
        dlist.insertBack(new int(val));
    }
    
    bool isEmpty()
    {
        return dlist.isEmpty();
    }
    
    void print()
    {
        cout << dlist << endl;
    }
};

class Queue 
{
private:
    Dlist<Instr> dlist;
    
public:
    Instr pop()
    {
        Instr *ptr = dlist.removeFront();
        Instr val = *ptr;
        delete ptr;
        
        return val;
    }
    
    void push(Instr val)
    {
        Instr *ptr = new Instr();
        (*ptr) = val;
        dlist.insertBack(ptr);
    }
    
    bool isEmpty()
    {
        return dlist.isEmpty();
    }
    
    void print()
    {
        cout << dlist << endl;
    }
};

int main(int argc, char *argv[])
{
    int n, m;
    
    cin >> n >> m;
    
    int mem[16];
    Stack st;
    Queue insQ;
    
    for (int i = 0; i < n; i++)
    {
        int x;
        cin >> x;
        st.push(x);
    }
    
    for (int i = 0; i < m; i++)
    {
        Instr ins;
        cin >> ins;
        insQ.push(ins);
    }
    
    for (int i = 0; i < 16; i++)
    {
        cin >> mem[i];
    }
    
    while (!insQ.isEmpty())
    {
        Instr ins = insQ.pop();
        
        
        auto print = [&]() -> void 
        {
            cout << ins << endl;
            st.print();
            insQ.print();
            for (int i = 0; i < 16; i++)
                cout << mem[i] << " ";
            cout << endl;
        };
        if (ins.name == InstrName::ADD)
        {
            int x = st.pop();
            int y = st.pop();
            st.push(x + y);
        }
        else if (ins.name == InstrName::NOR)
        {
            int x = st.pop();
            int y = st.pop();
            st.push(~(x | y));
        }
        else if (ins.name == InstrName::IFZ)
        {
            int x = st.pop();
            if (x == 0)
            {
                for (int i = 0; i < ins.parameter; i++)
                {
                    insQ.pop();
                }
            }
        }
        else if (ins.name == InstrName::HALT)
        {
            print();
            return 0;
        }
        else if (ins.name == InstrName::LOAD)
        {
            int address = st.pop();
            st.push(mem[address]);
        }
        else if (ins.name == InstrName::STORE)
        {
            int address = st.pop();
            int value = st.pop();
            mem[address] = value;
        }
        else if (ins.name == InstrName::POP)
        {
            st.pop();
        }
        else if (ins.name == InstrName::PUSHI)
        {
            st.push(ins.parameter);
        }
        else if (ins.name == InstrName::NOOP)
        {
        }
        
        print();
    }
    
    
    return 0;
}
