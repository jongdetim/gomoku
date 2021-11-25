#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

# include "gomoku.hpp"
# include "Board.hpp"
# include "TranspositionTable.hpp"

extern int TOTAL_NODES;
extern int TOTAL_LEAVES;
extern int FOUND_IN_TABLE;
extern int TOTAL_BRANCHES_PRUNED;

int negamax(Board node, int depth, int alpha, int beta, int color, TranspositionTable &t_table,TranspositionTable &_table, bool initial_call);

#endif
