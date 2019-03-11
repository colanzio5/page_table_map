/**
 * COLIN CASAZZA
 * CS570 - OPERATING SYSTEMS
 * CSSC1070
 * ASSIGNMENT THREE
 * 
 */
#include <iostream>
#include <tuple>
#include <string.h>
#include "pageTableShell.h"

int main()
{
    // int argc, char **argv
    // std::cout << "Starting Page Table Simulation\n";
    // std::cout << std::boolalpha;

    // std::tuple<int, std::string, bool, int> args = parseSimulationArguments(argc, argv);
    // std::cout << "first n entries to process    : " << std::get<0>(args) << "\n";
    // std::cout << "file to print to              : " << std::get<1>(args) << "\n";
    // std::cout << "show sim in console?          : " << (bool)std::get<2>(args) << "\n";
    // std::cout << "number of levels              : " << std::get<3>(args) << "\n";

    std::cout << "Total Addresses: " << runSimulation() << "\n";
    return 0;
}