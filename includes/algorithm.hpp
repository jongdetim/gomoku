#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

#include "gomoku.hpp"

extern bool TIMEOUT_REACHED;
const int TIMEOUT = 500;

class Board;
class TranspositionTable;
class Timer;

int negamax(Board node, int depth, int alpha, int beta, int player, TranspositionTable &t_table,TranspositionTable &h_table, bool initial_call, Timer &timer);

#endif
