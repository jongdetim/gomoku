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

inline bool	tt_lookup_is_valid(Board &node, TableEntry &tt_entry, int depth, TranspositionTable &t_table)
{
	return t_table.lookup(node, tt_entry) && tt_entry.depth >= depth;
}

void		set_tt_entry_values(TableEntry &tt_entry, int value, int alpha_orig, int beta, int depth, bool is_finished, int best_move)
{
	tt_entry.value = value;
	tt_entry.best_move = best_move;
	if (value <= alpha_orig)
		tt_entry.flag = UPPERBOUND;
	else if (value >= beta)
		tt_entry.flag = LOWERBOUND;
	else
		tt_entry.flag = EXACT;
	tt_entry.depth = depth;
	tt_entry.game_finished = is_finished;
}

int     	negamax(Board node, int depth, int alpha, int beta, int player, TranspositionTable &t_table, TranspositionTable &h_table, bool initial_call, Timer &timer)
{
	TableEntry tt_entry;
	int alpha_orig = alpha;
	int value = -std::numeric_limits<int>::max();
	bool is_finished;
	int best_move = -1;

	// check if timeout occured
	if (timer.elapsedMilliseconds() > TIMEOUT)
		throw "time limit reached";

	// (* Transposition Table Lookup; node is the lookup key for tt_entry *)
	if (tt_lookup_is_valid(node, tt_entry, depth, t_table))
	{
		FOUND_IN_TABLE++;
		if (tt_entry.flag == EXACT)
		{
			// std::cout << "EXACT FLAG FOUND IN TABLE " << std::endl;
			return tt_entry.value;
		}
		else if (tt_entry.flag == LOWERBOUND)
		{
			// std::cout << "LOWERBOUND FLAG FOUND IN TABLE " << std::endl;
			alpha = std::max(alpha, tt_entry.value);
		}
		else if (tt_entry.flag == UPPERBOUND)
		{
			// std::cout << "UPPERBOUND FLAG FOUND IN TABLE " << std::endl;
			beta = std::min(beta, tt_entry.value);
		}
		if (alpha >= beta)
			return tt_entry.value;
	}

	// this should happen during children node ordering, and should affect evaluation h to be + infinite
	// should also check if a player has won! currently only checks if board is completely full
	is_finished = node.is_game_finished();

	if (depth == 0 || is_finished)
	{
		TOTAL_LEAVES += 1;
		// node.print();

		// if (h_table.lookup(node, tt_entry))
		// 	std::cout << "impossible ding" << std::endl;
		value = -heuristic::get_heuristic_total(node);
		// PRINT(value);
		// value = color * node.calc_heuristic();

		// node.print();
		// PRINT("LEAF NODE");
		// std::cout << "value: " << value << std::endl;

		tt_entry.value = value;
		tt_entry.flag = EXACT;
		tt_entry.depth = depth;
		tt_entry.game_finished = is_finished;
		h_table.insert(node, tt_entry);
		t_table.insert(node, tt_entry);
		// std::cout << hashfn(node.get_state()) << std::endl;
		// std::cout << node.get_state() << std::endl;

		return (value);
	}
	std::vector<Board> child_nodes;
	// for (int i = 0; i < 100; i++)
		child_nodes = node.generate_children();

		// for (auto &it : child_nodes)
		// 	std::cout << it.last_move << std::endl;
		// exit(1);

	auto comp = [&](Board a, Board b)-> bool
	{
			return a.h > b.h;
	};

	// calculate heuristic for all the children to sort by. using lambda as comparison function to pass color param
	// if we've already seen the child in a shallower depth (previous search) we read the heuristic
	if (depth > 1)  // improve pruning by sorting leaf nodes based on cheap sorting heuristic. currently does not sort leaf nodes at all!
	{
		for (Board &child : child_nodes)
		{
			TableEntry ht_entry;
			if (h_table.lookup(child, ht_entry))
			{
				// std::cout << "already seen this child node" << std::endl;
				child.h = -ht_entry.value; //have to flip the sign again, because currently heuristics are stored with flipped sign at the leaf nodes
			}
			else
			{
			    // std::cout << "calculating child h" << std::endl;

				// child.h = -std::numeric_limits<int>::max();
			    child.h = heuristic::get_heuristic_total(child);
				// ht_entry.value = -child.h;
				// ht_entry.depth = depth - 1;
				// h_table.insert(child, ht_entry);
			}
		}
		std::sort(child_nodes.begin(), child_nodes.end(), comp);

		// std::cout << child_nodes[0].h << std::endl;
		// std::cout << (child_nodes.end() -1)->h << std::endl;
		// for (auto &it : child_nodes)
		// 	std::cout << it.get_last_move() << std::endl;
		// exit(1);
	}

	TOTAL_NODES += 1;
	
	best_move = child_nodes[0].get_last_move();
	for (Board child : child_nodes)
	{
		int old_value = value;

		if (child.check_free_threes(child.get_last_move(), child.get_last_player())) // Welke last move wil je hier hebben?
			continue;
		value = std::max(value, -negamax(child, depth - 1, -beta, -alpha, child.get_next_player(player), t_table, h_table, false, timer));
		int old_alpha = alpha;
		alpha = std::max(alpha, value);
		if (alpha > old_alpha)
		{
			// 'beta cutoff'?
		}
		if (value > old_value)
			best_move = child.get_last_move();

		if (node.get_state() == DEBUG_BOARD.get_state())
			PRINT("**** " << best_move << "*****");

		if (alpha >= beta)
		{
			// PV[depth] = child.get_last_move();
			TOTAL_BRANCHES_PRUNED++;
			// if (depth == 1)
			// 	std::cout << "PRUNED SOME LEAVES" << std::endl;
			break;
		}
	}
	// (* Transposition Table Store; node is the lookup key for tt_entry *)
	set_tt_entry_values(tt_entry, value, alpha_orig, beta, depth, is_finished, best_move);
	// PRINT(tt_entry.best_move);
	// if (t_table.lookup(node, tt_entry))
	// 	// t_table.update(node, value);
	// 	;
	// else
		t_table.insert(node, tt_entry);
		if (node.get_state() == DEBUG_BOARD.get_state())
		{
			t_table.lookup(node, tt_entry);
			PRINT("*****!!!!!!!   " << tt_entry.best_move);
		}
		// why does this improve performance???
		// h_table.insert(node, tt_entry);

	// this slightly reduces amount of visited nodes, but at the cost of table insertions. currently slows down the algo
	// 10-01-2022 why does this increase the amount of visited nodes now??
    TableEntry h_entry;
	
    if (h_table.lookup(node, h_entry))
	{
		// std::cout << value << std::endl;
		// node.print();
		// std::cout << h_entry.value << std::endl;
	    // h_table.update(node, value);
		// h_table.lookup(node, h_entry);
		// std::cout << "new value: " << h_entry.value << std::endl;
	}
	else //why does this improve performance????
	{
		// PRINT("this actually ever occurs");
		// node.show_last_move();
		h_entry.value = value;
		h_table.insert(node, h_entry);
	}

	if (initial_call)
	{
		// std::cout << "depth: " << depth << std::endl;
		// std::cout << "best move is: " << best_move << std::endl;
		// std::cout << "heuristic value is: " << value << std::endl;
		// if (best_move == -1)
		// 	print_and_quit("no best move found. something seriously wrong");
		// PRINT(best_move);
		return best_move;
	}
	return value;
}
