#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

#include <stdlib.h>
#include <vector>
#include <limits>
#include <iostream>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include "Board.hpp"
#include "heuristic.hpp"
#include "misc.hpp"

#define EXACT 0
#define LOWERBOUND 1
#define UPPERBOUND 2

// globals

    extern int TOTAL_NODES;
    extern int TOTAL_LEAVES;
	extern int FOUND_IN_TABLE;
	extern int TOTAL_BRANCHES_PRUNED;

class TableEntry
{
    public:
        int value;
        int depth;
        int flag;
        bool game_finished;
};

class TranspositionTable
{
    private:
        std::unordered_map<std::bitset<MASKSIZE>, TableEntry> t_table;

    public:
        bool lookup(Board &node, TableEntry &tt_entry);
        void insert(Board &node, TableEntry &tt_entry);
        size_t size();
};

int negamax(Board node, int depth, int alpha, int beta, int color, std::vector<int> filled_positions, TranspositionTable &t_table,TranspositionTable &_table);


#endif
