#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <tuple>
#include <string.h>

#include "pageTable.h"
#include "byutr.h"

// tuple contents
// nFlag (int - process only the first N memory requests)
// pFlag (FILE - the file to print the sim results to)
// tFlag (bool - flag to stdout the address mappings as added to table)
// number of levels,
// offset of the level size array in argv
std::tuple<int, std::string, bool, int, int> parseSimulationArguments(int argc, char **argv)
{
    int numberLevels = argc - 2;
    std::string pFlag = "NONE";
    bool tFlag = false;
    int nFlag = -1;
    int c = -1;

    /**
    * loop through all the arguments to the simulation
    * number of levels in our tree is the number of commands in args,
    * minus number of addition params: when param found (levels = argc - <size of param>)
    */
    while ((c = getopt(argc, argv, "tn:p:")) != -1)
    {
        switch (c)
        {
        // specify the the first N address to process
        case 'n':
            nFlag = atoi(optarg);
            numberLevels -= 2;
            break;

        case 'p':

            if (fopen(optarg, "r") == NULL)
            {
                printf("Error opening file!\n");
                exit(1);
            }
            else
            {
                pFlag = optarg;
                numberLevels -= 2;
            }
            break;

        case 't':
            tFlag = true;
            numberLevels--;
            break;

        default:
            break;
        }
    }
    return std::make_tuple(nFlag, pFlag, tFlag, numberLevels, argc - numberLevels);
}

// tuple contents
// nFlag (int - process only the first N memory requests)
// pFlag (FILE - the file path to print the sim results to)
// tFlag (bool - flag to stdout the address mappings as added to table)
// number of levels in page tree sim
// level size offset
int runSimulation(int argc, char **argv, std::tuple<int, std::string, bool, int, int> args)
{
    FILE *ifp;	        /* trace file */
    unsigned long i = 0;  /* instructions processed */
    char *tracePath = argv[argc - (int)(std::get<3>(args)) -1];
    p2AddrTr trace;	/* traced address */

    /* attempt to open trace file */
    if ((ifp = fopen("trace.sample.tr", "rb")) == NULL)
    {
        fprintf(stderr, "cannot open %s for reading\n", "trace.sample.tr");
        exit(1);
    }

    /**
     * initialize the page table given the input paramaters
     */
    int levels = (int)std::get<3>(args);
    int offset = (int)std::get<4>(args);
    PageTable pageTable = PageTable(levels, offset, argv);


    /**
     * using NextAddress() iterate through the trace file
     * for each address in the trace file, do a pageTable lookup
     * mark hits and misses, and add if doesn't exist
     */
    while (!feof(ifp))
    {
        /* get next address and process */
        if (NextAddress(ifp, &trace))
        {
            uint32_t address = (uint32_t)trace.addr;
            i++;
        }
    }
    return i;
}