#include "algorithm.hpp"

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
	return t_table.lookup(node, tt_entry) and tt_entry.depth >= depth;
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

int     	negamax(Board node, int depth, int alpha, int beta, int color, std::vector<int> filled_positions, TranspositionTable &t_table, TranspositionTable &h_table)
{
	TableEntry tt_entry;
	int alpha_orig = alpha;
	int value = -std::numeric_limits<int>::max();
	bool is_finished;

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
	is_finished = node.is_game_finished();
	if (depth == 0 or is_finished)
	{
		TOTAL_LEAVES += 1;
		// node.print();

		value = color * (calc_heuristic(node));
		tt_entry.value = value;
		tt_entry.flag = EXACT;
		tt_entry.depth = depth;
		tt_entry.game_finished = is_finished;
		h_table.insert(node, tt_entry);
		return (value);
	}

	filled_positions.push_back(node.last_move);
	// std::cout << "last_move: " << node.last_move << std::endl;
	std::vector<Board> child_nodes = node.generate_children(filled_positions, color);

	auto comp = [&](Board a, Board b)-> bool
	{
		// std::cout << "last_move: " << node.last_move << std::endl;
		if (color == PLAYER1)
			return a.h < b.h;
		else
			return a.h > b.h;
	};

	// calculate heuristic for all the children to sort by. using lambda as comparison function to pass color param
	if (depth > 1)
	{
		for (Board &child : child_nodes)
		{
			TableEntry tt_entry_lower;
			if (h_table.lookup(child, tt_entry_lower) and tt_entry_lower.depth < depth - 1)
			{
				// std::cout << "al gezien" << std::endl;
				child.h = -color * tt_entry_lower.value;
			}
			// else
			// {
			//     // std::cout << "calculating child h" << std::endl;
			//     child.h = calc_heuristic(child);
			// }
		}
		std::sort(child_nodes.begin(), child_nodes.end(), comp);
	}

	for (Board child : child_nodes)
	{
		TOTAL_NODES += 1;
		value = std::max(value, -negamax(child, depth - 1, -beta, -alpha, -color, filled_positions, t_table, h_table));
		alpha = std::max(alpha, value);
		if (alpha >= beta)
		{
			TOTAL_BRANCHES_PRUNED++;
			break;
		}
	}
	// (* Transposition Table Store; node is the lookup key for tt_entry *)
	set_tt_entry_values(tt_entry, value, alpha_orig, beta, depth, is_finished);
	t_table.insert(node, tt_entry);
	h_table.insert(node, tt_entry);
	return value;
}
