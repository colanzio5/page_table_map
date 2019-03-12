#include <iostream> 
#include <math.h>
#include "pageTable.h"


/*generates a bitmask of specified length 'length', and starting at the specified bit 'start'*/
unsigned int calcBitmask(int start, int length) {
    unsigned int mask = (1 << length) - 1; // (2^length) - 1
    mask <<= (start - length); //shifts the 1's to the correct starting position
    return mask;
}

PageTable::PageTable(int levels, int levelsArgsIndex, char **argv) 
{
    shift = (int*)calloc(levels, sizeof(int));
    bitMask = (uint32_t*)calloc(levels, sizeof(uint32_t));
    entries = (uint32_t*)calloc(levels, sizeof(uint32_t));
    
    int bitsUsed = 0;
    int i;
    for (i = 0; i < levels; i++) {
        int levelBits = atoi(argv[levelsArgsIndex]);
        shift[i] = ADDRESS_LENGTH - bitsUsed - levelBits;
        int maskStart = ADDRESS_LENGTH - bitsUsed; //calculate bitmask for current level
        bitMask[i] = calcBitmask(maskStart, levelBits);
        entries[i] = pow(2, levels);
        bitsUsed += levelBits;
        levelsArgsIndex++;
        // todo: see if this is necessairy
        if (bitsUsed >= ADDRESS_LENGTH) {
            fprintf(stderr,"Logical address length is too long. Max: 32 bits\n");
            exit(1);
        }
    }
    
    hits = 0;
    misses = 0;
    root = &Level::Level(this, &root, 0);

    // &Level(this, , 0);
}

int PageTable::getLevels(){
            return levels;
        }

int PageTable::getEntriesAtDepth(int depth){
    return entries[depth];
}

Level::Level(PageTable pageTable, Level *root, int level){
    depth = depth;
    isLeafNode = (depth+1 >= pageTable.getLevels());
    if (isLeafNode) {        //allocate maps for leaf nodes
        frame = (FrameMap*)calloc(pageTable.getEntriesAtDepth(depth), sizeof(FrameMap));
        int i;
        for (i = 0; i < pageTable.getEntriesAtDepth(depth); i++) {
            frame[i].isValid = false;
        }
    }
    else {          //allocate next level pointers
        nextLevel = (Level*)calloc(pageTable.getEntriesAtDepth(depth), sizeof(Level *));
    }
}

// class PageTable {

//     public: 
//         Level *root;
//         int levels;
//         int frameCount;
//         int bitsUsed;
//         int hits;
//         int misses;
//         uint32_t *bitMask;
//         uint32_t *entries;
//         int *shift;

        

        // int getLevels(){
        //     return levels;
        // }

        // int getEntriesAtDepth(int depth){
        //     return entries[depth];
        // }
// };


// class Level {

//     public:
//         bool isLeafNode;
//         struct PageTable *pageTable;
//         struct Level *nextLevel;
//         struct FrameMap *frame;
//         int depth;

        
// };