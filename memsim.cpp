#include "fifo.hpp"
#include "lru.hpp"
#include "vms.hpp"
using namespace std;

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

    return 0;
}
