#include "vms.hpp"

void segmentedFifo(int frameNumber, vector<Pairs> &addr_map, string mode, const int partition){
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
    // for (int i = 0; i < 1000; i++)
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
            char tempValue = fifoMemory[temp]; // seems to be working fine
            // cout << tempValue << "\n"; 
            // updating the primary first

            if (fifoMemory[fifoBuffer.front()] == 'W')
            { // incrementing disk writes
                diskWrites++; // test1 ^^^^ : definately wrong bc this alone produces 750 writes!!!!!!
            }

            fifoMemory.erase(fifoBuffer.front());         // removing from primary
            fifoBuffer.pop();                             // updating primary page table
            fifoMemory[address] = addr_map[i].getValue(); // putting in the referenced element
            fifoBuffer.push(address);                     // updating the page table again


            //forgot to remove the most recent element from secondary...
            lruMemory.erase(address);


            if (lruMemory.size() < lruCapacity)
            {                                                // lru buffer has space and we just insert the element in it
                lruMemory[temp] = tempValue; //inserting the value
                indexMap[temp] = i;                       //updating the indexmap
                // diskReads++; //test2 ^^^^^


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
                } // This one is just adding 1 extra write
                diskReads++; // test3 ^^^^
                // Remove the indexes page
                lruMemory.erase(val);
                // insert the current page
                lruMemory[temp] = tempValue;
                indexMap[temp] = i;
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
                char temp1Value = fifoMemory[temp1];
                // updating the primary first
                if (fifoMemory[fifoBuffer.front()] == 'W')
                { // incrementing disk writes
                    diskWrites++;
                }
                fifoMemory.erase(fifoBuffer.front());         // removing from primary
                fifoBuffer.pop();                             // updating primary page table
                fifoMemory[address] = addr_map[i].getValue(); // putting in the referenced element
                fifoBuffer.push(address);
                



                // we move on to figure out where to store the temp element
                if (lruMemory.size() < lruCapacity) //secondary has space
                {                                                // lru buffer has space and we just insert the element in it
                    lruMemory[temp1] = temp1Value; //inserting the value
                    indexMap[temp1] = i;                       //updating the indexmap
                    // diskReads++;

                    
                }
                else
                { // no space, needs replacement
                    // Find the least recently used pages
                    // that is present in the set
                    // diskReads++;// memory access still
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
                    } //This is not doing anything ***********

                    // Remove the indexes page
                    lruMemory.erase(val);
                    // insert the current page
                    lruMemory[temp1] = temp1Value;
                    indexMap[temp1] = i;
                }
            }
        }
    }

    std::cout << "Total memory frames: " << frameNumber << "\n";
    std::cout << "events in trace: " << addr_map.size() << "\n";
    std::cout << "Total disk reads: " << diskReads << "\n";
    std::cout << "Total disk writes: " << diskWrites << "\n";
}