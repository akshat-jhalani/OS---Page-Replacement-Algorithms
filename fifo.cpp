#include "fifo.hpp"

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