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
	filled_positions.push_back(node.last_move);

	if (depth == 0 || is_finished)
	{
		TOTAL_LEAVES += 1;
		// node.print();

		value = color * calc_heuristic_tim(filled_positions, node);

		// node.print();
		// std::cout << "value: " << value * color << std::endl;

		tt_entry.value = value;
		tt_entry.flag = EXACT;
		tt_entry.depth = depth;
		tt_entry.game_finished = is_finished;
		h_table.insert(node, tt_entry);
		t_table.insert(node, tt_entry);
		return (value);
	}
	
	// std::cout << "last_move: " << node.last_move << std::endl;
	std::vector<Board> child_nodes = node.generate_children(filled_positions, color);

	auto comp = [&](Board a, Board b)-> bool
	{
			return a.h > b.h;
	};

	// calculate heuristic for all the children to sort by. using lambda as comparison function to pass color param
	// if we've already seen the child in a shallower depth (previous search) we read the heuristic && multiply by
	// -color to accommodate the fact that leaf nodes are stored 1 recursion deeper than the children are generated in
	if (depth > 1)
	{
		for (Board &child : child_nodes)
		{
			TableEntry ht_entry;
			if (h_table.lookup(child, ht_entry))
			{
				// dit gebeurt nooit?
				// std::cout << "al gezien" << std::endl;
				child.h = -ht_entry.value;
			}
			else
			{
			    // std::cout << "calculating child h" << std::endl;
			    child.h = color * calc_heuristic_tim_from_parent(filled_positions, child);
				// ht_entry.value = child.h;
				// ht_entry.depth = depth - 1;
				// h_table.insert(child, ht_entry);
			}
		}
		std::sort(child_nodes.begin(), child_nodes.end());
	}

	TOTAL_NODES += 1;

	for (Board child : child_nodes)
	{
		value = std::max(value, -negamax(child, depth - 1, -beta, -alpha, -color, filled_positions, t_table, h_table));
		alpha = std::max(alpha, value);
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
	t_table.insert(node, tt_entry);

	// this seems to have no practical effect (yet)
    TableEntry h_entry;
    if (h_table.lookup(node, h_entry))
	    h_table.update(node, value);

	return value;
}
