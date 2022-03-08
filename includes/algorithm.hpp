#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

#include "gomoku.hpp"

extern bool TIMEOUT_REACHED;
const int TIMEOUT = 500;
const int NARROWING[5] = {30, 20, 15, 10, 5};

class Board;
class TranspositionTable;
class Timer;

<<<<<<< HEAD
int negamax(Board node, int depth, int initial_depth, int alpha, int beta, int color, TranspositionTable &t_table,TranspositionTable &h_table, bool initial_call, Timer &timer);

extern Board DEBUG_BOARD;
=======
int negamax(Board node, int depth, int alpha, int beta, int player, TranspositionTable &t_table,TranspositionTable &h_table, bool initial_call, Timer &timer);
>>>>>>> gui

#endif
