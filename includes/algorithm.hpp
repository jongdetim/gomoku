#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

#include <stdlib.h>
#include <vector>
#include <limits>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "Board.hpp"

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

int negamax(Board node, int depth, int alpha, int beta, int color, std::vector<int> filled_positions, TranspositionTable &t_table);


#endif
