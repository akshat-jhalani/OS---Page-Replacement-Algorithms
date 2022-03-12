#include "lru.hpp"

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
            }
            pageTable.push(key);
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
            }
            pageTable.push(key);
            std::cout << "Total memory frames: " << frameNumber << endl;
            std::cout << "Events in trace: " << addr_map.size() << endl;
            std::cout << "Total disk reads: " << diskReads << endl;
            std::cout << "Total disk writes: " << diskWrites << endl;
        }
    }
}