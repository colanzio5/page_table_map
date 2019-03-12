#ifndef PAGE_TABLE
#define PAGE_TABLE

#include <tuple>
#define ADDRESS_LENGTH 32


class PageTable {


    public: 
        struct Level *root;
        int levels;
        int frameCount;
        int bitsUsed;
        int hits;
        int misses;
        uint32_t *bitMask;
        uint32_t *entries;
        int *shift;

        PageTable(int levels, int levelsArgsIndex, char **argv);
        int getEntriesAtDepth(int depth);
        int getLevels();
};

class Level {

    public: 
        bool isLeafNode;
        struct PageTable *pageTable;
        struct Level **nextLevel;
        struct FrameMap *frame;
        int depth;

        Level(PageTable pageTable, Level *root, int level);
            
};

typedef struct FrameMap {        //leaf node structre
    bool isValid;
    uint32_t frame;
} FrameMap;


/*generates a bitmask of specified length 'length', and starting at the specified bit 'start'*/
unsigned int calcBitmask(int start, int length);

#endif // PAGE_TABLE