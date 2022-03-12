#ifndef DS_HPP
#define DS_HPP
#include <iostream>
#include <fstream>
#include <map>
#include <iterator>
#include <queue>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <unordered_map>
using namespace std;

class Pairs
{
private:
    unsigned key;
    char value;

public:
    unsigned getKey()
    {
        return this->key;
    }
    char getValue()
    {
        return this->value;
    }

    void setKey(unsigned keyPassed)
    {
        this->key = keyPassed;
    }

    void setValue(char valuePassed)
    {
        this->value = valuePassed;
    }

    void setKV(unsigned addr, char rw)
    {
        this->setKey(addr);
        this->setValue(rw);
    }
};

class NewMap
{
public:
    vector<Pairs> map;
    void insert(Pairs pair)
    {
        map.push_back(pair);
    }
    void remove()
    {
        map.pop_back();
    }

    bool isWrite()
    {
        char rw = this->map.back().getValue();
        if (rw == 'W')
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    void removeLast()
    {
        this->map.pop_back();
    }
    void printList()
    {
        for (int i = 0; i < this->map.size(); ++i)
        {
            std::cout << hex << this->map[i].getKey() << ": " << this->map[i].getValue() << "\n";
        }
    }
};

#endif