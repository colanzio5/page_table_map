#include <iostream>
#include <math.h>
#include "pageTable.h"

u_int32_t calcBitmask(int start, int length)
{
    u_int32_t mask = pow(2, length) - 1;
    mask <<= (start - length);
    return mask;
}

/* returns the index of a particular level of the page tree (given the bitmask and the number of bits to shit right
 * i.e. logicalToPage(0x7B93E1A5, 0x0FF00000, 20) would return 0xB9
 */
u_int32_t logicalToPage(u_int32_t logicalAddress, u_int32_t bitmask, u_int32_t shift)
{
    u_int32_t page = logicalAddress;
    page &= bitmask;
    page >>= shift;
    return page;
}

int initPageTable(char **argv, int argvOffset, PAGETABLE *pageTable)
{
    (*pageTable).shift = (int *)calloc((*pageTable).numLevels, sizeof(int));
    (*pageTable).entries = (int *)calloc((*pageTable).numLevels, sizeof(int));
    (*pageTable).mask = (u_int32_t *)calloc((*pageTable).numLevels, sizeof(u_int32_t));

    int tableSize = 0;
    int i;
    for (i = 0; i < (*pageTable).numLevels; i++)
    {
        int levelSize = atoi(argv[argvOffset]);
        int maskStart = ADDRESS_LENGTH - tableSize;
        (*pageTable).shift[i] = ADDRESS_LENGTH - tableSize - levelSize;
        (*pageTable).mask[i] = calcBitmask(maskStart, levelSize);
        (*pageTable).entries[i] = pow(2, levelSize);
        tableSize += levelSize;
        argvOffset++;
        if (tableSize >= ADDRESS_LENGTH)
        {
            exit(1);
        }
    }

    (*pageTable).hits = 0;
    (*pageTable).misses = 0;
    (*pageTable).root = initLevel(pageTable, (*pageTable).root, 0);
    return tableSize;
}

/* allocates space for a new level in the page table */
LEVEL *initLevel(PAGETABLE *pageTable, LEVEL *level, int depth)
{
    level = (LEVEL *)calloc(1, sizeof(LEVEL));
    (*level).pageTable = pageTable;
    (*level).depth = depth;
    (*level).isLeaf = (depth + 1 >= (*pageTable).numLevels);
    if ((*level).isLeaf)
    {
        (*level).map = (MAP *)calloc((*pageTable).entries[depth], sizeof(MAP));
        int i;
        for (i = 0; i < (*pageTable).entries[depth]; i++)
        {
            (*level).map[i].valid = false;
        }
    }
    else
    {
        (*level).nextLevel = (LEVEL **)calloc((*pageTable).entries[depth], sizeof(LEVEL *));
    }
    return level;
}


MAP *pageLookup(PAGETABLE *pageTable, unsigned int logicalAddress)
{
    return searchLevel(pageTable->root, logicalAddress);
}

/* searches for a given logical address and returns a pointer to the map struct associated with the address, or NULL if address not found */
MAP *searchLevel(LEVEL *level, unsigned int logicalAddress)
{
    u_int32_t mask = level->pageTable->mask[(*level).depth];
    u_int32_t shift = level->pageTable->shift[(*level).depth];
    u_int32_t index = logicalToPage(logicalAddress, mask, shift);

    if (level->isLeaf)
    {
        if (level->map[index].valid)
        {

            return &level->map[index];
        }
        else
        {

            return NULL;
        }
    }
    else
    {
        if (level->nextLevel[index] == NULL)
        {

            return NULL;
        }
        return searchLevel(level->nextLevel[index], logicalAddress);
    }
}

/* calls pageInsertHelper() on the root node */
void pageInsert(PAGETABLE *pageTable, u_int32_t logicalAddress, u_int32_t frame)
{
    pageInsertHelper((*pageTable).root, logicalAddress, frame);
}

/* inserts the given logical address into the page table, and creates new levels when needed */
void pageInsertHelper(LEVEL *level, u_int32_t logicalAddress, u_int32_t frame)
{

    u_int32_t mask = level->pageTable->mask[(*level).depth];
    u_int32_t shift = level->pageTable->shift[(*level).depth];
    u_int32_t index = logicalToPage(logicalAddress, mask, shift);

    if ((*level).isLeaf)
    { //test if current level is a leaf node
        (*level).map[index].valid = true;
        (*level).map[index].frame = frame;
        level->pageTable->numFrames++;
    }
    else
    { //traverse to the next level. Create a new level if nextLevel[index] is NULL
        if ((*level).nextLevel[index] == NULL)
            (*level).nextLevel[index] = initLevel((*level).pageTable, level, (*level).depth + 1);
        pageInsertHelper((*level).nextLevel[index], logicalAddress, frame);
    }
}

/* prints general information about the page table */
void printTableInfo(PAGETABLE *pageTable)
{
    std::cout << "page table info: " << pageTable->numLevels << "\n";
    int i;
    for (i = 0; i < pageTable->numLevels; i++)
    {
        printf("LEVEL %i INFO: ", i);
        printf("Mask: %08X\tShift: %i\tEntry Count: %i\n", pageTable->mask[i], pageTable->shift[i], pageTable->entries[i]);
    }
}