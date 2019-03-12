#ifndef PAGE_TABLE_SHELL
#define PAGE_TABLE_SHELL

#include <tuple>
#define ADDRESS_LENGTH 32

// tuple contents
// nFlag (int - process only the first N memory requests)
// pFlag (FILE - the file to print the sim results to)
// tFlag (bool - flag to stdout the address mappings as added to table)
// number of levels,
// offset of the level size array in argv
std::tuple<int, std::string, bool, int, int> parseSimulationArguments(int argc, char **argv);

int runSimulation(int argc, char **argv, std::tuple<int, std::string, bool, int, int> args);

#endif // PAGE_TABLE_SHELL