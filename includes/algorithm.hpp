/* MIT License

Copyright (c) 2022 Flint Louis, Tim de Jong

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
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

t_search_results	negamax(Board node, int depth, int initial_depth, int alpha, int beta, int color, TranspositionTable &t_table, Timer &timer, std::atomic<bool> *stop_search);
void				set_aistats(t_aistats &aistats, int depth, int heuristic, int duration);

#endif
