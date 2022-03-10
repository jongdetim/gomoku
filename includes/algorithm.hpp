#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

#include "gomoku.hpp"

extern bool TIMEOUT_REACHED;
const int TIMEOUT = 500;
const int NARROWING[6] = {30, 30, 15, 15, 8, 8};

class Board;
class TranspositionTable;
class Timer;

typedef struct	s_aistats
{
	int			depth;
	int			duration;
	int			nodes_visited;
	int			score;
}				t_aistats;

class NegamaxAi
{
public:
	int	calculate(Board &board, t_aistats &aistats);

private:
    int iterative_deepening_negamax(Board board, int player, t_aistats &aistats);
};

int		negamax(Board node, int depth, int initial_depth, int alpha, int beta, int color, TranspositionTable &t_table,TranspositionTable &h_table, bool initial_call, Timer &timer);
void	set_aistats(t_aistats &aistats, int depth, int heuristic, int duration);

#endif
