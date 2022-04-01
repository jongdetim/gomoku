#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

#include "gomoku.hpp"
#include <atomic>

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
	int			move;
}				t_aistats;

typedef struct	s_search_results
{
	int 		heuristic;
	int			best_move;
	bool		is_finished;
}				t_search_results;

class NegamaxAi
{
public:
	t_aistats		calculate_move(Board board, int timeout, std::atomic<int> *move_highlight, std::atomic<bool> *stop_search);

private:
	int				iterative_deepening_negamax(Board board, int player, t_aistats &aistats, int timeout, std::atomic<int> *move_highlight, std::atomic<bool> *stop_search);
};

t_search_results	negamax(Board node, int depth, int initial_depth, int alpha, int beta, int color, TranspositionTable &t_table, TranspositionTable &h_table, Timer &timer, std::atomic<bool> *stop_search);
void				set_aistats(t_aistats &aistats, int depth, int heuristic, int duration);

#endif
