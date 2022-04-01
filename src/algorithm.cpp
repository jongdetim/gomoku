/* MIT License

Copyright (c) 2022 Flint Louis

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
#include "algorithm.hpp"
#include "Board.hpp"
#include "misc.hpp"
#include "heuristic.hpp"
#include "TranspositionTable.hpp"
#include "gomoku.hpp"

int TOTAL_LEAVES = 0;
int TOTAL_NODES = 0;
int	FOUND_IN_TABLE = 0;
int TOTAL_BRANCHES_PRUNED = 0;
bool TIMEOUT_REACHED = false;

t_aistats	NegamaxAi::calculate_move(Board board, int timeout, std::atomic<int> *move_highlight, std::atomic<bool> *stop_search)
{
	t_aistats stats = {0,0,0,0,0};

	if (board.is_empty())
		stats.move = *move_highlight = misc::calc_index(9,9);
	else
		stats.move = this->iterative_deepening_negamax(board, board.get_current_player(), stats, timeout, move_highlight, stop_search);
	return stats;
}

void		reset_globals()
{
	TOTAL_LEAVES = 0;
	TOTAL_NODES = 0;
	FOUND_IN_TABLE = 0;
	TOTAL_BRANCHES_PRUNED = 0;
	TIMEOUT_REACHED = false;
}

int			NegamaxAi::iterative_deepening_negamax(Board board, int player, t_aistats &aistats, int timeout, std::atomic<int> *move_highlight, std::atomic<bool> *stop_search)
{
	Timer timer(timeout);
	t_search_results results;
	t_search_results last_results;
	TranspositionTable h_table;
	TranspositionTable t_table;

	int depth = 1;
	int max_depth = 100;
	reset_globals();	
	timer.start();
	
	for (; depth <= max_depth && !TIMEOUT_REACHED; depth++)
	{
		try
		{
			last_results = negamax(board, depth, depth, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), player, t_table, h_table, timer, stop_search);
			*move_highlight = last_results.best_move;
		}
		catch(const char* e)
		{
			depth--;
			break;
		}
		results = last_results;
		if (results.is_finished)
			break;
	}
	set_aistats(aistats, depth, results.heuristic, timer.elapsed_ms);
	return results.best_move;
}

void		set_aistats(t_aistats &aistats, int depth, int heuristic, int duration)
{
	aistats.depth = depth;
	aistats.duration = duration;
	aistats.score = heuristic;
	aistats.nodes_visited = TOTAL_NODES;
}

inline bool	tt_lookup_is_valid(Board &node, TableEntry &tt_entry, int depth, TranspositionTable &t_table)
{
	return t_table.retrieve(node, tt_entry) && tt_entry.depth >= depth;
}

void		set_tt_entry_values(TableEntry &tt_entry, t_search_results &results, int alpha_orig, int beta, int depth)
{
	tt_entry.results = results;
	if (results.heuristic <= alpha_orig)
		tt_entry.flag = UPPERBOUND;
	else if (results.heuristic >= beta)
		tt_entry.flag = LOWERBOUND;
	else
		tt_entry.flag = EXACT;
	tt_entry.depth = depth;
}

int			branch_narrowing(int depth)
{
	if (depth > 5)
		return 3;
	else
		return NARROWING[depth];
}

t_search_results     	negamax(Board node, int depth, int initial_depth, int alpha, int beta, int player, TranspositionTable &t_table, TranspositionTable &h_table, Timer &timer, std::atomic<bool> *stop_search)
{
	TableEntry tt_entry;
	t_search_results results;

	int alpha_orig = alpha;
	int value = -std::numeric_limits<int>::max();
	int best_move = -1;

	// check if timeout occured
	if (timer.timeout_reached() || *stop_search)
		throw "time limit reached or search stopped";

	TOTAL_NODES += 1;

	// Transposition Table Lookup; node.zobrist_hash is the lookup key for tt_entry
	if (tt_lookup_is_valid(node, tt_entry, depth, t_table))
	{
		FOUND_IN_TABLE++;
		if (tt_entry.flag == EXACT)
			return tt_entry.results;
		else if (tt_entry.flag == LOWERBOUND)
			alpha = std::max(alpha, tt_entry.results.heuristic);
		else if (tt_entry.flag == UPPERBOUND)
			beta = std::min(beta, tt_entry.results.heuristic);
		if (alpha >= beta)
			return tt_entry.results;
	}

	// this should happen during children node ordering, and should affect evaluation h to be + infinite
	results.is_finished = node.is_game_finished(1 - player);

	// leaf nodes
	if (depth == 0 || results.is_finished)
	{
		TOTAL_LEAVES += 1;

		if (results.is_finished)
		{
			if (node.winner == player)
				results.heuristic = WINNING_POINTS[0] - (initial_depth - depth);
			else if (node.winner == 1 - player)
				results.heuristic = -(WINNING_POINTS[0] - (initial_depth - depth));
			else
				results.heuristic = 0; //board is full
		}
		else
			results.heuristic = -heuristic::get_heuristic_total(node, 1 - player);

		results.best_move = -1;

		tt_entry.results = results;
		tt_entry.flag = EXACT;
		tt_entry.depth = depth;
		h_table.insert(node, tt_entry);
		t_table.insert(node, tt_entry);

		return (results);
	}

	std::vector<Board> child_nodes = node.generate_children(player);

	TableEntry pv;
	bool node_seen_before = false;
	if (t_table.retrieve(node, pv) && pv.flag == EXACT)
		node_seen_before = true;

	// PV goes to the front of the queue. Using PV search, the performance is WORSE! the best line of previous depth is not the first we want to explore APPARENTLY
	auto comp = [&node_seen_before, &pv](Board &a, Board &b)-> bool
	{
		if (a.get_last_move() == b.get_last_move())
			return false;
		if (node_seen_before && a.get_last_move() == pv.results.best_move)
			return true;
		return a.h > b.h;
	};

	// calculate heuristic for all the children to sort by. using lambda as comparison function to pass color param
	// if we've already seen the child in a shallower depth (previous search) we read the heuristic
	if (depth > 1)  // improve pruning by sorting leaf nodes based on cheap sorting heuristic. currently does not sort leaf nodes at all!
	{
		for (Board &child : child_nodes)
		{
			TableEntry ht_entry;
			if (h_table.retrieve(child, ht_entry))
				child.h = -ht_entry.results.heuristic; //have to flip the sign again, because currently heuristics are stored with flipped sign at the leaf nodes
			else
			{
			    child.h = heuristic::get_heuristic_total(child, player);
			    // child.h = -color * node.calc_heuristic(child);
				// ht_entry.value = child.h;
				// ht_entry.depth = depth - 1;
				// h_table.insert(child, ht_entry);
			}
		}
		std::sort(child_nodes.begin(), child_nodes.end(), comp);
	}

	best_move = child_nodes[0].get_last_move();
	bool is_finished = false;

	for (Board child : child_nodes)
	{
		int old_value = value;

		if (child.is_free_threes(child.get_last_move(), child.get_last_player())) // Welke last move wil je hier hebben?
			continue;
		results = negamax(child, depth - 1, initial_depth, -beta, -alpha, 1 - player, t_table, h_table, timer, stop_search);
		value = std::max(value, -results.heuristic);
		alpha = std::max(alpha, value);
		if (value > old_value)
		{
			best_move = child.get_last_move();
			is_finished = results.is_finished;
		}
		if (alpha >= beta)
		{
			TOTAL_BRANCHES_PRUNED++;
			break;
		}
	}

	results.heuristic = value;
	results.best_move = best_move;
	results.is_finished = is_finished;

	set_tt_entry_values(tt_entry, results, alpha_orig, beta, depth);

	if (t_table.contains(node))
		t_table.update(node, tt_entry);
	else
		t_table.insert(node, tt_entry);
	
    if (h_table.contains(node)) // still not sure if updating values improves performance
	    h_table.update(node, tt_entry);
	else
		h_table.insert(node, tt_entry);
	return results;
}
