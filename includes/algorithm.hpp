#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

# include "gomoku.hpp"
# include "Board.hpp"
# include "TranspositionTable.hpp"
# include "misc.hpp"

extern bool TIMEOUT_REACHED;
const int TIMEOUT = 500;

int negamax(Board node, int depth, int alpha, int beta, int color, TranspositionTable &t_table,TranspositionTable &h_table, bool initial_call, Timer &timer);

#endif
