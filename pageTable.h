#ifndef PAGE_TABLE
#define PAGE_TABLE

#define ADDRESS_LENGTH 32

typedef struct PAGETABLE {
    struct LEVEL *root;
    int numLevels;
    int numFrames;
    int hits;
    int misses;
    u_int32_t *mask;
    int *shift;
    int *entries;

} PAGETABLE;

typedef struct MAP {        //leaf node structre
    bool valid;
    u_int32_t frame;
} MAP;

typedef struct LEVEL {      //interior LEVEL structure
    bool isLeaf;
    struct PAGETABLE *pageTable;
    struct LEVEL **nextLevel;
    struct MAP *map;
    int depth;
} LEVEL;

u_int32_t calcBitmask(int start, int length);

u_int32_t logicalToPage(u_int32_t logicalAddress, u_int32_t bitmask, u_int32_t shift);

int initPageTable(char **argv, int argvOffset, PAGETABLE *pageTable);

LEVEL * initLevel(PAGETABLE *pageTable, LEVEL *level, int depth);

void pageInsert(PAGETABLE *pageTable, u_int32_t logicalAddress, u_int32_t frame);

void pageInsertHelper(LEVEL *level, u_int32_t logicalAddress, u_int32_t frame);

MAP *pageLookup(PAGETABLE *pageTable, u_int32_t logicalAddress);

MAP *searchLevel(LEVEL *level, unsigned int logicalAddress);

void printTableInfo(PAGETABLE *pageTable);

#endif // PAGE_TABLE