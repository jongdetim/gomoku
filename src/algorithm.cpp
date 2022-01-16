#include "algorithm.hpp"
#include "Board.hpp"
#include "Heuristic.hpp"
#include "TranspositionTable.hpp"

int TOTAL_LEAVES = 0;
int TOTAL_NODES = 0;
int	FOUND_IN_TABLE = 0;
int TOTAL_BRANCHES_PRUNED = 0;

// bool order_moves(std::vector<Board> &nodes, TranspositionTable &t_table, int color)
// {
// 	return true;
//     if (color == 1);
// }

inline bool	tt_lookup_is_valid(Board &node, TableEntry &tt_entry, int depth, TranspositionTable &t_table)
{
	return t_table.lookup(node, tt_entry) && tt_entry.depth >= depth;
}

void		set_tt_entry_values(TableEntry &tt_entry, int value, int alpha_orig, int beta, int depth, bool is_finished)
{
	tt_entry.value = value;
	if (value <= alpha_orig)
		tt_entry.flag = UPPERBOUND;
	else if (value >= beta)
		tt_entry.flag = LOWERBOUND;
	else
		tt_entry.flag = EXACT;
	tt_entry.depth = depth;
	tt_entry.game_finished = is_finished;
}

int     	negamax(Board node, int depth, int alpha, int beta, int color, TranspositionTable &t_table, TranspositionTable &h_table, bool initial_call)
{
	TableEntry tt_entry;
	int alpha_orig = alpha;
	int value = -std::numeric_limits<int>::max();
	bool is_finished;
	int best_move = -1;
	std::hash<std::bitset<722>> hashfn;

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
		value = color * node.calc_heuristic();

		// node.print();
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
		child_nodes = node.generate_children(color);

		// for (auto &it : child_nodes)
		// 	std::cout << it.last_move << std::endl;
		// exit(1);

	auto comp = [&](Board a, Board b)-> bool
	{
			return a.h < b.h;
	};

	// calculate heuristic for all the children to sort by. using lambda as comparison function to pass color param
	// if we've already seen the child in a shallower depth (previous search) we read the heuristic
	if (depth > 1)
	{
		for (Board &child : child_nodes)
		{
			TableEntry ht_entry;
			if (h_table.lookup(child, ht_entry))
			{
				// std::cout << "al gezien" << std::endl;
				// child.h = ht_entry.value * pow(color, depth - ht_entry.depth);
				child.h = ht_entry.value;
			}
			else
			{
			    // std::cout << "calculating child h" << std::endl;

			    child.h = -color * node.calc_heuristic(child);
				ht_entry.value = child.h;
				ht_entry.depth = depth - 1;
				h_table.insert(child, ht_entry);
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

	for (Board child : child_nodes)
	{
		int old_value = value;

		if (child.check_free_threes(child.get_last_move(), color))
			continue;
		value = std::max(value, -negamax(child, depth - 1, -beta, -alpha, -color, t_table, h_table, false));
		alpha = std::max(alpha, value);
		if (initial_call && value > old_value)
			best_move = child.get_last_move();
		if (alpha >= beta)
		{
			TOTAL_BRANCHES_PRUNED++;
			// if (depth == 1)
			// 	std::cout << "PRUNED SOME LEAVES" << std::endl;
			break;
		}
	}
	// (* Transposition Table Store; node is the lookup key for tt_entry *)
	set_tt_entry_values(tt_entry, value, alpha_orig, beta, depth, is_finished);
	if (t_table.lookup(node, tt_entry))
		t_table.update(node, value);
	else
		t_table.insert(node, tt_entry);

	// this slightly reduces amount of visited nodes, but at the cost of table insertions. currently slows down the algo
	// 10-01-2022 why does this increase the amount of visited nodes now??
    TableEntry h_entry;
    // if (h_table.lookup(node, h_entry))
	// {
	// 	// std::cout << value << std::endl;
	// 	// node.print();
	// 	// std::cout << h_entry.value << std::endl;
	//     h_table.update(node, value);
	// 	// h_table.lookup(node, h_entry);
	// 	// std::cout << "new value: " << h_entry.value << std::endl;
	// }
	if (initial_call)
	{
		std::cout << "depth: " << depth << std::endl;
		std::cout << "best move is: " << best_move << std::endl;
		std::cout << "heuristic value is: " << value << std::endl;
		node.place(best_move, color);
		node.show_last_move();
		// std::cout << node.calc_heuristic() << std::endl;
	}
	return value;
}
