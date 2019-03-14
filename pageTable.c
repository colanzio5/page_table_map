#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <math.h>
#include "pageTable.h"

ARGS* initArgs(int argc, char **argv) {
    ARGS *args = (ARGS *)calloc(1, sizeof(ARGS));
    int levels = argc-2;
    int opt;
    while((opt = getopt(argc, argv, "n:p:t")) != -1) {
        switch(opt) {
            case 'n':
                args->nFlag = atoi(optarg);
                levels -= 2;
                break;

            case 'p':
                args->pFlag = 1;
                FILE *printFile;
                args->printFilePath=optarg;
                levels -= 2;
                break;
            case 't':
                args->tFlag = 1;
                levels--;
                break;
            default:
                break;
        }
    }
    args->numLevels = levels;
    args->level = (int*)calloc(levels, sizeof(int));
    for(int i = 0; i< levels; i++){
        int levelSize = atoi(argv[argc - levels + i]);
        args->level[i] = levelSize;
    }
    return args;
}

PAGETABLE *initPageTable(PAGETABLE *pageTable, int *levels, int numLevels)
{
    // init page table properties
    pageTable->levelCount = numLevels;
    pageTable->hits = 0;
    pageTable->misses = 0;
    pageTable->frameCount = 0;
    // init memory for page table arrays
    pageTable->bitMaskAry = (uint32_t *)calloc(numLevels, sizeof(uint32_t));
    pageTable->shiftAry = (unsigned long *)calloc(numLevels, sizeof(unsigned long));
    pageTable->entryCount = (unsigned long *)calloc(numLevels, sizeof(unsigned long));

    // init page table root arrays
    int totalBitsUsed = 0;
    for (int i = 0; i < numLevels; i++)
    {
        uint32_t mask = calcBitmask(ADDRESS_LENGTH - totalBitsUsed, levels[i]);
        pageTable->bitMaskAry[i] = mask;
        pageTable->shiftAry[i] = ADDRESS_LENGTH - totalBitsUsed - levels[i];
        pageTable->entryCount[i] = pow(2, levels[i]);
        totalBitsUsed += levels[i];
    }
    pageTable->root = initLevel(pageTable, 0, levels[0]);
    pageTable->bytesUsed = sizeof(pageTable);
    return pageTable;
}

LEVEL *initLevel(PAGETABLE *pageTable, int depth, int size)
{
    LEVEL *level = (LEVEL *)calloc(1, sizeof(LEVEL));
    level->depth = depth;
    level->pageTable = pageTable;
    if ((depth + 1 >= pageTable->levelCount))
    {
        // if it's a leaf node, init all the map frames
        level->map = (MAP *)calloc(pageTable->entryCount[depth], sizeof(MAP));
        for (int i = 0; i < pageTable->entryCount[depth]; i++)
        {
            level->map[i].isValid = 0;
            pageTable->bytesUsed += sizeof(level);
        }
    }
    // otherwise init a level that all null
    else
    {
        level->nextLevel = (LEVEL **)calloc(pageTable->entryCount[depth], sizeof(LEVEL *));
    }
    // pageTable->totalBytes += sizeof(level);
    return level;
}

// calls page insert on the root node of the page table
MAP* pageInsert(PAGETABLE *pageTable, uint32_t logicalAddress, uint32_t frame)
{
    pageInsertHelper(pageTable->root, logicalAddress, frame);
}
// inserts a page into the level, if the level is a leaf node
// if the level is not a leaf node, traverse to the next level
// if the next level is not initialized, create a new level
MAP* pageInsertHelper(LEVEL *level, uint32_t logicalAddress, uint32_t frame)
{

    PAGETABLE *pageTable = level->pageTable;
    uint32_t mask = pageTable->bitMaskAry[level->depth];
    unsigned long shift = pageTable->shiftAry[level->depth];
    int index = logicalToPage(logicalAddress, mask, shift);
    // if the level is a leaf node, add to the frame
    if (level->depth + 1 >= pageTable->levelCount)
    {
        level->map[index].isValid = 1;
        level->map[index].frame = frame;
        level->pageTable->frameCount++;
        return &level->map[index];
    }
    // otherwise, traverse to the next level,
    // if next level DNE, create it
    else
    {
        if (level->nextLevel[index] == NULL)
        {
            int newLevelDepth = level->depth + 1;
            int newLevelSize = pageTable->entryCount[level->depth + 1];
            level->nextLevel[index] = initLevel(pageTable, newLevelDepth, newLevelSize);
        }
        pageInsertHelper(level->nextLevel[index], logicalAddress, frame);
    }
}

// call pageLookup on the root level of the given page table
MAP *pageLookup(PAGETABLE *pageTable, uint32_t logicalAddress)
{
    return pageLookupHelper(pageTable->root, logicalAddress);
}
// traverse each level, if the level is a leaf node and if so search for frame
// if level is not a leaf node, recurse to the next level
MAP *pageLookupHelper(LEVEL *level, uint32_t logicalAddress)
{
    PAGETABLE *pageTable = level->pageTable;
    uint32_t mask = pageTable->bitMaskAry[level->depth];
    unsigned long shift = pageTable->shiftAry[level->depth];
    int index = logicalToPage(logicalAddress, mask, shift);

    // if the level is a leaf node, search for the frame
    if (level->depth + 1 >= pageTable->levelCount)
    {
        if (level->map[index].isValid == 1)
        {
            MAP *map = &level->map[index];
            return &level->map[index];
        }
        else
            return NULL;
    }
    // otherwise traverse to the next level and search
    else
    {
        if (level->nextLevel[index] == NULL)
            return NULL;

        return pageLookupHelper(level->nextLevel[index], logicalAddress);
    }
}

// from a given logical address, returns the index of the pageTable's lookup information
// for any given logical address
// (given logical address A, the page for A in pageTable is
// at mask,shift,entries[logicalToPage(A)])
int logicalToPage(uint32_t logicalAddress, uint32_t bitmask, unsigned int shift)
{
    uint32_t page = logicalAddress;
    page &= bitmask;
    page >>= shift;
    return page;
}

uint32_t calcBitmask(int start, int length)
{
    uint32_t mask = (1 << length) - 1; // (2^length) - 1
    mask <<= (start - length);         //shifts the 1's to the correct starting position
    return mask;
}