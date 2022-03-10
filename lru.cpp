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
            cout << hex << this->map[i].getKey() << ": " << this->map[i].getValue() << "\n";
        }
    }
};

// function to implement lru cache replacement algorithm
void lru(int frameNumber, int pageSize, vector<Pairs> &addr_map){
    // create a queue to store the pages
    queue<unsigned int> pageTable;
    // set total memory frames to 64
    // read map implementing lru cache and counting page faults, disk read/writes
    int pageFaults = 0;
    int diskReads = 0;
    int diskWrites = 0;

    // create a map to store the page table
    map<unsigned int, int> pageTableMap;
    // create a map to store the page table
    map<unsigned int, int>::iterator it;

    // append addr_map to page table map
    for (int i = 0; i < addr_map.size(); i++){
        pageTableMap.insert(pair<unsigned int, int>(addr_map[i].getKey(), i));
    }

    // iterate through the page table map, counting page faults and disk reads/writes
    for (it = pageTableMap.begin(); it != pageTableMap.end(); it++){
        // if the page is not in the page table, increment page faults
        if (pageTable.size() < frameNumber){
            pageTable.push(it->first);
            pageFaults++;
            if (addr_map[it->second].getValue() == 'W'){
                diskWrites++;
            }
            else{
                diskReads++;
            }
        }
        // if the page is in the page table, remove it from the page table
        else{
            pageTable.pop();
            pageTable.push(it->first);
            if (addr_map[it->second].getValue() == 'W'){
                diskWrites++;
            }
            else{
                diskReads++;
            }
        }
    }


    // print the page faults
    cout << "Page faults: " << pageFaults << endl;
    // print the disk reads
    cout << "Disk reads: " << diskReads << endl;
    // print the disk writes
    cout << "Disk writes: " << diskWrites << endl;
    
}



int main(int argc, char const *argv[])
{
    ifstream file;
    file.open("sixpack.trace");
    unsigned int address;
    char value;
    Pairs KVpair;
    NewMap refList;
    // total events
    int totalEvents = 0;
    while (file >> hex >> address >> value)
    {
        KVpair.setKV(address, value);
        refList.insert(KVpair);
        totalEvents++;
    }

    //   refList.printList();
    // print total events
    cout << "Total events: " << totalEvents << endl;
    lru(64, 4096, refList.map);


    return 0;
}


