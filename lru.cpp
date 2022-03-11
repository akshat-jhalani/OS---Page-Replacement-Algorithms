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

// function to implement lru cache replacement algorithm
void lru(int frameNumber, vector<Pairs> &addr_map, string mode)
{
    // create a queue to store the pages
    queue<unsigned int> pageTable;
    // set total memory frames to 64
    // read map implementing lru cache and counting page faults, disk read/writes
    int pageFaults = 0;
    int diskReads = 0;
    int diskWrites = 0;
    int traceCount = 0;

    // create a map to store the page table
    map<unsigned int, char> memory;
    // create a map to store the page table
    map<unsigned int, char>::iterator it;

    //
    map<unsigned int, int> indexMap;
    map<unsigned int, int>::iterator it1;

    // MODE: debug or quiet
    if (mode == "quiet")
    {
        std::cout << "QUIET MODE" << endl;
        // iterate through the address map
        for (int i = 0; i < addr_map.size(); i++)
        {
            // get the key and value from the map
            unsigned int key = addr_map[i].getKey();
            char value = addr_map[i].getValue();

            key /= 4096;

            if (memory.size() < frameNumber) // memory has space
            {
                if (memory.find(key) == memory.end()) //page not in memory
                {
                    memory[key] = value;
                    diskReads++;
                    cout << "Page faults: " << pageFaults << "\n";
                }

                // Store the recently used index of
                // each page
                indexMap[key] = i;
            }
            else // when the memory is at capacity
            {
                if (memory.find(key) == memory.end()) // page not in memory
                {

                    // Find the least recently used pages
                    // that is present in the set
                    diskReads++;
                    cout << "dr: " << diskReads << endl;
                    int lru = INT_MAX, val;
                    for (it = memory.begin(); it != memory.end(); it++)
                    {
                        if (indexMap[it->first] < lru)
                        {
                            lru = indexMap[it->first];
                            val = it->first;
                        }
                    }

                    if (memory[val] == 'W')
                    {
                        diskWrites++;
                    }
                    // Remove the indexes page
                    memory.erase(val);

                    // insert the current page
                    memory[key] = value;

                    // Increment page faults
                    pageFaults++;
                }
                // Update the current page index
                indexMap[key] = i;
                if (memory[key] == 'R')
                {
                    memory[key] = addr_map[i].getValue();
                }
            }
            // pageTable.push(key);
        }
        std::cout << "Total memory frames: " << frameNumber << endl;
        std::cout << "events in trace: " << addr_map.size() << endl;
        std::cout << "Total disk reads: " << diskReads << endl;
        std::cout << "Total disk writes: " << diskWrites << endl;
    }
    else if (mode == "debug")
    {
        std::cout << "DEBUG MODE" << endl;
        // iterate through the address map
        for (int i = 0; i < addr_map.size(); i++)
        {
            // get the key and value from the map
            unsigned int key = addr_map[i].getKey();
            char value = addr_map[i].getValue();

            key /= 4096;

            if (memory.size() < frameNumber)
            {
                if (memory.find(key) == memory.end())
                {
                    memory[key] = value;
                    // increment page fault
                    pageFaults++;
                }

                // Store the recently used index of
                // each page
                indexMap[key] = i;
            }
            // If the set is full then need to perform lru
            // i.e. remove the least recently used page
            // and insert the current page
            else
            {
                if (memory.find(key) == memory.end())
                {

                    // Find the least recently used pages
                    // that is present in the set
                    diskReads++;
                    int lru = INT_MAX, val;
                    for (it1 = indexMap.begin(); it1 != indexMap.end(); it++)
                    {
                        if (indexMap[it->first] < lru)
                        {
                            lru = indexMap[it->first];
                            val = it->first;
                        }
                    }

                    if (memory[val] == 'W')
                    {
                        diskWrites++;
                    }

                    // Remove the indexes page
                    memory.erase(val);

                    // insert the current page
                    memory[key] = value;

                    // Increment page faults
                    pageFaults++;
                }
                // Update the current page index
                indexMap[key] = i;
            }
            pageTable.push(key);
            std::cout << "Total memory frames: " << frameNumber << endl;
            std::cout << "Events in trace: " << addr_map.size() << endl;
            std::cout << "Total disk reads: " << diskReads << endl;
            std::cout << "Total disk writes: " << diskWrites << endl;
        }
    }
}

void fifo(const int frameNumber, int pageSize, vector<Pairs> &addr_map, string mode)
{
    int pageFaults = 0;
    int diskReads = 0;
    int diskWrites = 0;

    // addr_map is just a vector<pairs>

    std::queue<unsigned int> pageTable;
    std::map<unsigned int, char> memory;

    if (mode == "quiet")
    {

        for (int i = 0; i < addr_map.size(); i++)
        {
            unsigned offset = addr_map[i].getKey();
            offset = offset / 4096; // adjusting the address offset

            if (memory.size() == 0)
            { // trying to resolve seg fault
                goto initial;
            }

            // is the address is found in tfound in the memory map:
            if (memory.find(offset) != memory.end())
            {
                if (memory[offset] == 'R')
                {
                    memory[offset] = addr_map[i].getValue();
                }
            }

            // page not found in memory
            else
            {
            initial:
                // need to access disk:

                diskReads++;
                // if the page table has space:
                if (frameNumber > pageTable.size()) // 0 < 64...63 < 64
                {
                    pageTable.push(offset);
                    memory[offset] = addr_map[i].getValue();
                }
                else
                { // The memory has no room
                    // checking if the element to be replaced is dirty:
                    if (memory[pageTable.front()] == 'W')
                    {
                        diskWrites++;
                    }
                    // removing the oldest element from the pageTable and the memory
                    memory.erase(pageTable.front());
                    pageTable.pop();
                    // adding the new element
                    pageTable.push(offset);
                    memory[offset] = addr_map[i].getValue();
                }
            }
        }
        std::cout << "Total memory frames: " << frameNumber << "\n";
        std::cout << "events in trace: " << addr_map.size() << "\n";
        std::cout << "Total disk reads: " << diskReads << "\n";
        std::cout << "Total disk writes: " << diskWrites << "\n";
    }

    else if (mode == "debug")
    {

        for (int i = 0; i < addr_map.size(); i++)
        {

            unsigned offset = addr_map[i].getKey();
            offset = offset / 4096; // adjusting the address offset

            if (memory.size() == 0)
            { // trying to resolve seg fault
                goto initialDebug;
            }

            // is the address is found in tfound in the memory map:
            if (memory.find(offset) != memory.end())
            {
                if (memory[offset] == 'R')
                {
                    memory[offset] = addr_map[i].getValue();
                }
            }

            // page not found in memory
            else
            {
            initialDebug:
                // need to access disk:
                diskReads++;
                // if the page table has space:
                if (frameNumber > pageTable.size()) // 0 < 64...63 < 64
                {
                    pageTable.push(offset);
                    memory[offset] = addr_map[i].getValue();
                }
                else
                { // The memory has no room
                    // checking if the element to be replaced is dirty:
                    if (memory[pageTable.front()] == 'W')
                    {
                        diskWrites++;
                    }
                    // removing the oldest element from the pageTable and the memory
                    memory.erase(pageTable.front());
                    pageTable.pop();
                    // adding the new element
                    pageTable.push(offset);
                    memory[offset] = addr_map[i].getValue();
                }
            }
            std::cout << "events in trace: " << i << "\n";
            std::cout << "Total disk reads: " << diskReads << "\n";
            std::cout << "Total disk writes: " << diskWrites << "\n";
        }
        std::cout << "\n"
                     "\n"
                     "Total memory frames: "
                  << frameNumber << "\n";
        std::cout << "events in trace: " << addr_map.size() << "\n";
        std::cout << "Total disk reads: " << diskReads << "\n";
        std::cout << "Total disk writes: " << diskWrites << "\n";
    }
}

void segmentedFifo(int frameNumber, vector<Pairs> &addr_map, string mode, const int partition)
{

    // form two queues - fifo and lru with each of their capacity based on the partition
    std::queue<unsigned int> fifoBuffer;
    std::queue<unsigned int> lruBuffer;
    map<unsigned int, int> indexMap; // this is the buffer for lru
    map<unsigned int, int>::iterator it;
    std::map<unsigned int, char> fifoMemory;
    std::map<unsigned int, char> lruMemory;
    int diskReads = 0;
    int diskWrites = 0;

    const int lruCapacity = (frameNumber * partition) / 100; // since P represents percentage of memory alloted to the secondary buffer
    const int fifoCapacity = frameNumber - lruCapacity;      // primary buffer

    for (int i = 0; i < addr_map.size(); i++)
    {
        unsigned int address = addr_map[i].getKey();
        address = address / 4096; // adjusting the offset

        // if the reference is present in the Primary or Secondary buffer
        if (fifoMemory.find(address) != fifoMemory.end()) //checking if present in the primary/fifo
        { // No reads since already in the buffer, no writes since we are just returning
            if (fifoMemory[address] == 'R')
            {
                fifoMemory[address] = addr_map[i].getValue(); // no need to update page table
            }
        }

        

        else if (lruMemory.find(address) != lruMemory.end()) // element in the secondary buffer - need to take that page and push to primary/fifo
        {                                                    // 
            // if some element is in the secondary buffer, that means that the primary is full every time so no need to check
            unsigned temp = fifoBuffer.front(); // the element to be moved to lru buffer
            // updating the primary first
            if (fifoMemory[fifoBuffer.front()] == 'W')
            { // incrementing disk writes
                diskWrites++; // test1 ^^^^
            }
            fifoMemory.erase(fifoBuffer.front());         // removing from primary
            fifoBuffer.pop();                             // updating primary page table
            fifoMemory[address] = addr_map[i].getValue(); // putting in the referenced element
            fifoBuffer.push(address);                     // updating the page table again

            // incorrect logic here ^^^^^^^^^^^^^^^^^^^
            
            if (lruMemory.size() < lruCapacity)
            {                                                // lru buffer has space and we just insert the element in it
                lruMemory[address] = addr_map[i].getValue(); //inserting the value
                indexMap[address] = i;                       //updating the indexmap
                diskReads++; //test2 ^^^^^


            }
            else // moving the least recent element to the disk
            { // no space, needs replacement
                // Find the least recently used pages
                // that is present in the set
                // diskReads++;
                int lru = INT_MAX, val;
                for (it = indexMap.begin(); it != indexMap.end(); it++)
                {
                    if (indexMap[it->first] < lru)
                    {
                        lru = indexMap[it->first];
                        val = it->first;
                    }
                }
                // if there is a dirty page, increment the disk writes
                if (lruMemory[val] == 'W')
                {
                    diskWrites++;
                }
                diskReads++; // test3 ^^^^
                // Remove the indexes page
                lruMemory.erase(val);
                // insert the current page
                lruMemory[temp] = addr_map[i].getValue();
            }
        }



        else // if the element is not present in the primary or secondary buffer
        {
            diskReads++; //Accessing memory regardless
            // then we need to check if the primary buffer is full or not
            if (fifoBuffer.size() < fifoCapacity) // put in primary if it has space
            {
                fifoBuffer.push(address);
                fifoMemory[address] = addr_map[i].getValue();
                // diskReads++;
            }
            else
            { // if it is full, juggle from the primary to the secondary

                unsigned temp1 = fifoBuffer.front(); // the element to be moved to lru buffer
                // updating the primary first
                // if (fifoMemory[fifoBuffer.front()] == 'W')
                // { // incrementing disk writes
                //     diskWrites++;
                // }
                fifoMemory.erase(fifoBuffer.front());         // removing from primary
                fifoBuffer.pop();                             // updating primary page table
                fifoMemory[address] = addr_map[i].getValue(); // putting in the referenced element
                fifoBuffer.push(address);
                



                // we move on to figure out where to store the temp element
                if (lruMemory.size() < lruCapacity) //secondary has space
                {                                                // lru buffer has space and we just insert the element in it
                    lruMemory[temp1] = addr_map[i].getValue(); //inserting the value
                    indexMap[temp1] = i;                       //updating the indexmap
                    // diskReads++;

                    
                }
                else
                { // no space, needs replacement
                    // Find the least recently used pages
                    // that is present in the set
                    diskReads++;// memory access still
                    int lru = INT_MAX, val;
                    for (it = indexMap.begin(); it != indexMap.end(); it++)
                    {
                        if (indexMap[it->first] < lru)
                        {
                            lru = indexMap[it->first];
                            val = it->first;
                        }
                    }

                    if (lruMemory[val] == 'W')
                    {
                        diskWrites++;
                    }

                    // Remove the indexes page
                    lruMemory.erase(val);
                    // insert the current page
                    lruMemory[temp1] = addr_map[i].getValue();
                }
            }
        }
    }

    std::cout << "Total memory frames: " << frameNumber << "\n";
    std::cout << "events in trace: " << addr_map.size() << "\n";
    std::cout << "Total disk reads: " << diskReads << "\n";
    std::cout << "Total disk writes: " << diskWrites << "\n";
}

int main(int argc, char const *argv[])
{
    if (argc < 5)
    {
        std::cout << "Incorrect number of arguments!";
        std::cout << "The program requires exactly 5(6 in case of vms) arguments in the format:";
        std::cout << "memsim <tracefile> <nframes> <lru|fifo|vms> <p(only needed for vms)> <debug|quiet>";
        return 0;
    }
    string traceFile = argv[1];
    int nFrames = atoi(argv[2]);
    string algoName = argv[3];
    int partition = 0;
    string mode;
    if (algoName == "vms")
    {
        partition = atoi(argv[4]);
        mode = argv[5];
    }
    else
    {
        mode = argv[4];
    }

    ifstream file;
    file.open(traceFile);
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
    // cout << "Total events: " << totalEvents << endl;

    // starting the needed algorithm
    if (algoName == "fifo")
    {
        fifo(nFrames, 4096, refList.map, mode);
    }
    else if (algoName == "lru")
    {
        lru(nFrames, refList.map, mode);
    }
    else if (algoName == "vms")
    {
        segmentedFifo(nFrames, refList.map, mode, partition);
    }
    else
    {
        std::cout << "Invalid argument for argument name!";
        std::cout << "Please enter one of the three <lru|fifo|vms>";
        return 0;
    }

    // lru(64, refList.map, mode);
    // fifo(64, 4096, refList.map, mode);

    return 0;
}
