#include <stdio.h>
#include <stdlib.h>


#include "byutr.h"

int runSimulation(const char *tracePath) 
{
    
    FILE *traceFile;
    p2AddrTr trace;

    if ((traceFile = fopen(tracePath,"rb")) == NULL) {
        exit(1);
    }
    
    unsigned long addressCount = 0;
    while (!feof(traceFile)) {
        if (NextAddress(traceFile, &trace)) {
            addressCount = addressCount  + 1;
        }
    }
    return addressCount;
}