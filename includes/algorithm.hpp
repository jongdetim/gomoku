#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

#include "gomoku.hpp"

extern bool TIMEOUT_REACHED;
const int TIMEOUT = 2500;

class Board;
class TranspositionTable;
class Timer;

int negamax(Board node, int depth, int alpha, int beta, int color, TranspositionTable &t_table,TranspositionTable &h_table, bool initial_call, Timer &timer);

extern Board DEBUG_BOARD;

#endif
