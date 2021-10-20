#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

#include <stdlib.h>
#include <vector>
#include <limits>
#include <iostream>
#include <algorithm>
#include <functional>
#include "Board.hpp"
#include "heuristic.hpp"
#include "misc.hpp"
#include "TranspositionTable.hpp"

#define EXACT 0
#define LOWERBOUND 1
#define UPPERBOUND 2

// globals
extern int TOTAL_NODES;
extern int TOTAL_LEAVES;
extern int FOUND_IN_TABLE;
extern int TOTAL_BRANCHES_PRUNED;

int negamax(Board node, int depth, int alpha, int beta, int color, std::vector<int> filled_positions, TranspositionTable &t_table,TranspositionTable &_table, bool initial_call);


#endif
