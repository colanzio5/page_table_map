/**
 * COLIN CASAZZA
 * CS570 - OPERATING SYSTEMS
 * CSSC1070
 * ASSIGNMENT THREE
 * 
 */
#include <iostream>
#include "helpers.h"

int main(int argc, char **argv)
{
    std::cout << "Starting Page Table Simulation\n";
    std::cout << "Total Addresses: " << runSimulation("./trace/trace.sample.tr") << "\n";
    return 0;
}
    