#ifndef PAGE_TABLE_SHELL
#define PAGE_TABLE_SHELL

#include <tuple>

// tuple contents
// nFlag (int - process only the first N memory requests)
// pFlag (FILE - the file to print the sim results to)
// tFlag (bool - flag to stdout the address mappings as added to table)
std::tuple<int, std::string, bool, int> parseSimulationArguments(int argc, char **argv);

int runSimulation();

#endif // PAGE_TABLE_SHELL