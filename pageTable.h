#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#define ADDRESS_LENGTH 32

typedef struct ARGS
{
    int nFlag = -1;
    int pFlag = 0;
    int tFlag = 0;
    int numLevels;
    int *level;
    char *printFilePath;
} ARGS;

typedef struct PAGETABLE
{
    int levelCount;
    int frameCount;
    int hits;
    int misses;
    int bytesUsed;

    uint32_t *bitMaskAry;
    unsigned long *shiftAry;
    unsigned long *entryCount;
    struct LEVEL *root;
} PAGETABLE;

typedef struct MAP
{
    int isValid;
    uint32_t frame;
} MAP;

typedef struct LEVEL
{
    PAGETABLE *pageTable;
    LEVEL **nextLevel;
    struct MAP *map;
    int depth;
} LEVEL;

// init functions
ARGS* initArgs(int argc, char **argv);
PAGETABLE* initPageTable(PAGETABLE *pageTable, int *levels, int numLevels);
LEVEL* initLevel(PAGETABLE *pageTable, int depth, int size);

// mutatation functions
MAP* pageInsert(PAGETABLE *pageTable, uint32_t logicalAddress, uint32_t frame);
MAP* pageInsertHelper(LEVEL *level, uint32_t logicalAddress, uint32_t frame);

// query functions
MAP * pageLookup(PAGETABLE *pageTable, uint32_t logicalAddress);
MAP * pageLookupHelper(LEVEL *level, uint32_t logicalAddress);

// helper functions
int logicalToPage(uint32_t logicalAddress, uint32_t bitmask, unsigned int shift);
uint32_t calcBitmask(int start, int length);

#endif //PAGE_TABLE_H