#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

#include "gomoku.hpp"

extern bool TIMEOUT_REACHED;
const int TIMEOUT = 5500;
const int NARROWING[6] = {30, 30, 15, 15, 8, 8};

class Board;
class TranspositionTable;
class Timer;

int negamax(Board node, int depth, int initial_depth, int alpha, int beta, int color, TranspositionTable &t_table,TranspositionTable &h_table, bool initial_call, Timer &timer);

extern Board DEBUG_BOARD;

#endif
