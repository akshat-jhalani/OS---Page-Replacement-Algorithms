
# Operating Systems Project 2 - Measuring the Performance of Page Replacement Algorithms on Real Traces

To build the project, run

    $ make

To execute the project, run

    $ ./memsim <tracefile> <nframes> <lru|fifo|vms> <p(only needed for vms)> <debug|quiet> 

For example:
    
    $ ./memsim "./traces/bzip.trace" 64 lru debug

Trace files to test are in the ./traces directory.

