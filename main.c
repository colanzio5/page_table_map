/**
 * COLIN CASAZZA
 * CS570 - OPERATING SYSTEMS
 * CSSC1070
 * ASSIGNMENT THREE - PAGETABLE
 * 3/13/19
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "byutr.h"
#include "pageTable.h"

int main(int argc, char **argv)
{

    // init variables and allocat storage for files and pagetable
    ARGS *args = initArgs(argc, argv);
    PAGETABLE *pageTable = (PAGETABLE *)calloc(1, sizeof(PAGETABLE));
    pageTable = initPageTable(pageTable, args->level, args->numLevels);
    FILE *traceFile;
    FILE *printFile;

    // open the provided tracefile
    char *tracePath = argv[argc-(args->numLevels)-1];
    if ((traceFile = fopen(tracePath, "rb")) == NULL)
    {
        fprintf(stderr, "cannot open %s for reading\n", tracePath);
        exit(1);
    }

    // if the pflag is present init the page table 
    if (args->pFlag == 1)
    {
        if ((printFile = fopen(args->printFilePath, "w")) == NULL)
        {
            fprintf(stderr, "cannot open %s for reading\n", args->printFilePath);
            exit(1);
        }
    }

    // start looping through the addresses in the tracefile
    unsigned long addressCount = 0;
    p2AddrTr trace;
    while (!feof(traceFile))
    {
        if (NextAddress(traceFile, &trace))
        {
            addressCount++;
            uint32_t address = (uint32_t)trace.addr;
            MAP *lookup = (MAP*)calloc(1, sizeof(MAP));
            // if the pageLookup is present we've missed,
            // iterate the misses, then add page to the table
            if (pageLookup(pageTable, address) == NULL)
            {
                pageTable->misses++;
                lookup = pageInsert(pageTable, address, pageTable->frameCount);
                // if the pflag is present, print the successfully added address
                if (args->pFlag == 1)
                    fprintf(printFile, "%08X -> %08X\n", address, lookup->frame);
            }
            // otherwise the page is already in the table and we have a hit
            else
            {
                pageTable->hits++;
                lookup = pageLookup(pageTable, address);
            }
            // if the tflag is present, output the current logical to phyisical address
            if (args->tFlag == 1)
                printf("%08x -> %08x\n", address, lookup->frame);
            // if the nflag is present, exit the process when we've exceeded the limit
            if (args->nFlag != -1 && addressCount > (args->nFlag - 1))
                break;
        }
    }

    // print out information about the run
    int hitPercent = (float)pageTable->hits / (float)addressCount * 100;
    int missPercent = (float)pageTable->misses / (float)addressCount * 100;
    printf("# Addresses Processed: %lu\n", addressCount);
    printf("Page Size: %i\n", pageTable->frameCount);
    printf("Bytes Used: %i\n", pageTable->bytesUsed);
    printf("Hits: %i (%i%%)\n", pageTable->hits, hitPercent);
    printf("Misses: %i (%i%%)\n", pageTable->misses, missPercent);

    // if the pflag was present sort the print file by addresses
    // then close the printfile
    if(args->pFlag == 1)
    {
        char command[5000];
        sprintf(command, "sort -k1 -o %s %s", args->printFilePath, args->printFilePath);
        system(command);
        fclose(printFile);
    }
    fclose(traceFile);
    return 0;
}
