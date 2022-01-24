#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

# include "gomoku.hpp"
# include "Board.hpp"
# include "TranspositionTable.hpp"

extern int TOTAL_NODES;
extern int TOTAL_LEAVES;
extern int FOUND_IN_TABLE;
extern int TOTAL_BRANCHES_PRUNED;
// extern std::chrono::milliseconds TIMEOUT;
extern bool TIMEOUT_REACHED;

int negamax(Board node, int depth, int alpha, int beta, int color, TranspositionTable &t_table,TranspositionTable &h_table, bool initial_call);

#endif
