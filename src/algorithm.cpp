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

t_aistats	NegamaxAi::calculate(Board board)
{
	t_aistats stats = {0,0,0,0,0};

	if (board.is_empty())
		stats.move = misc::calc_index(9,9);
	else
		stats.move = this->iterative_deepening_negamax(board, board.get_current_player(), stats);
	return stats;
}

int			NegamaxAi::iterative_deepening_negamax(Board board, int player, t_aistats &aistats)
{
	TOTAL_LEAVES = 0;
	TOTAL_NODES = 0;
	FOUND_IN_TABLE = 0;
	TOTAL_BRANCHES_PRUNED = 0;
	TIMEOUT_REACHED = false;

	Timer timer;
	t_search_results results;
	t_search_results last_results;
	TranspositionTable h_table;
	TranspositionTable t_table;

	int depth = 1;
	int max_depth = 100;
	
	timer.start();
	
	for (; depth <= max_depth && !TIMEOUT_REACHED; depth++)
	{
		try
		{
			last_results = negamax(board, depth, depth, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), player, t_table, h_table, true, timer);
		}
		catch(const char* e)
		{   
			// PRINT("depth: " << depth - 1);
			// board.print_principal_variation(player, depth - 1, t_table);
			// PRINT("timeout reached during depth: " << depth << ".\nusing previous depth search results:");
			
			// PRINT("heuristic: " << tt_entry.value);
			// PRINT(timer.elapsed_ms);
			// return results.best_move;
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

// just for debugging
bool PV_TABLE;

t_search_results     	negamax(Board node, int depth, int initial_depth, int alpha, int beta, int player, TranspositionTable &t_table, TranspositionTable &h_table, bool initial_call, Timer &timer)
{
	TableEntry tt_entry;
	t_search_results results;

	int alpha_orig = alpha;
	int value = -std::numeric_limits<int>::max();
	int best_move = -1;

	if (initial_call)
		PV_TABLE = true;
	// check if timeout occured
	if (timer.elapsedMilliseconds() >= TIMEOUT)
		throw "time limit reached";

	TOTAL_NODES += 1;

	// (* Transposition Table Lookup; node is the lookup key for tt_entry *)
	if (tt_lookup_is_valid(node, tt_entry, depth, t_table))
	{
		FOUND_IN_TABLE++;
		if (tt_entry.flag == EXACT)
		{
			// std::cout << "EXACT FLAG FOUND IN TABLE " << std::endl;
			return tt_entry.results;
		}
		else if (tt_entry.flag == LOWERBOUND)
		{
			// std::cout << "LOWERBOUND FLAG FOUND IN TABLE " << std::endl;
			alpha = std::max(alpha, tt_entry.results.heuristic);
		}
		else if (tt_entry.flag == UPPERBOUND)
		{
			// std::cout << "UPPERBOUND FLAG FOUND IN TABLE " << std::endl;
			beta = std::min(beta, tt_entry.results.heuristic);
		}
		if (alpha >= beta)
			return tt_entry.results;
	}

	// this should happen during children node ordering, and should affect evaluation h to be + infinite
	// should also check if a player has won! currently only checks if board is completely full
	results.is_finished = node.is_game_finished(1 - player);


	if (depth == 0 || results.is_finished)
	{
		TOTAL_LEAVES += 1;
		// node.print();

		PV_TABLE = false;
		// if (h_table.lookup(node, tt_entry))
		// 	std::cout << "impossible ding" << std::endl;
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
		// PRINT(results.heuristic);
		// results.heuristic = color * node.calc_heuristic();

		// node.print();
		// PRINT("LEAF NODE");
		// std::cout << "results.heuristic: " << results.heuristic << std::endl;

		results.best_move = -1;

		tt_entry.results = results;
		tt_entry.flag = EXACT;
		tt_entry.depth = depth;
		h_table.insert(node, tt_entry);
		t_table.insert(node, tt_entry);
		// std::cout << hashfn(node.get_state()) << std::endl;
		// std::cout << node.get_state() << std::endl;

		return (results);
	}

	std::vector<Board> child_nodes = node.generate_children(player);

	TableEntry pv;
	bool node_seen_before = false;
	if (PV_TABLE && t_table.retrieve(node, pv) && pv.flag == EXACT)
		node_seen_before = true;

	// PV goes to the front of the queue. Using PV search, the performance is WORSE! the best line of previous depth is not the first we want to explore APPARENTLY
	auto comp = [/* &node_seen_before, &pv ==> UNUSED <== */](Board &a, Board &b)-> bool
	{
		// if (a.get_last_move() == b.get_last_move())
		// 	return false;
		// if (node_seen_before && a.get_last_move() == pv.best_move)
		// 	return true;
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
			{
				// std::cout << "already seen this child node" << std::endl;
				child.h = -ht_entry.results.heuristic; //have to flip the sign again, because currently heuristics are stored with flipped sign at the leaf nodes
			}
			else
			{
			    // std::cout << "calculating child h" << std::endl;

			    child.h = heuristic::get_heuristic_total(child, player);
			    // child.h = -color * node.calc_heuristic(child);
				// ht_entry.value = child.h;
				// ht_entry.depth = depth - 1;
				// h_table.insert(child, ht_entry);
			}
			// if (node_seen_before && child.get_last_move() == pv.best_move)
			// {
			// 	PRINT(depth << "PV FOUND");
			// 	child.show_last_move();
			// }
		}
		std::sort(child_nodes.begin(), child_nodes.end(), comp);

		// if (node_seen_before && child_nodes[0].get_last_move() == pv.best_move)
		// 	PRINT(depth << "pv node :DDD");
		// std::cout << child_nodes[0].h << std::endl;
		// std::cout << (child_nodes.end() -1)->h << std::endl;
		// for (auto &it : child_nodes)
		// 	std::cout << it.get_last_move() << std::endl;
		// exit(1);
	}
	// PRINT("curent depth:" << depth);
	// if (t_table.retrieve(node, pv) && pv.flag == EXACT)
	// {
	// 	PRINT("depth: " << depth << "\nbest move: " << child_nodes[0].get_last_move());
	// 	PRINT("PV move: " << pv.best_move);
	// }
	
	best_move = child_nodes[0].get_last_move();
	bool is_finished = false;

	for (Board child : child_nodes)
	{
		// child.show_last_move();
		// PRINT(child.zobrist_hash);
		// if (counter >= branch_narrowing(initial_depth - depth))
		// 	break;
		int old_value = value;

		if (child.is_free_threes(child.get_last_move(), child.get_last_player())) // Welke last move wil je hier hebben?
			continue;
		results = negamax(child, depth - 1, initial_depth, -beta, -alpha, 1 - player, t_table, h_table, false, timer);
		value = std::max(value, -results.heuristic);
		// int old_alpha = alpha; ==> UNUSED <==
		alpha = std::max(alpha, value);
		if (value > old_value)
		{
			best_move = child.get_last_move();
			is_finished = results.is_finished;
		}

		// if (node.get_state() == DEBUG_BOARD.get_state())
		// 	PRINT("**** " << best_move << "*****");

		if (alpha >= beta)
		{
			TOTAL_BRANCHES_PRUNED++;
			// if (depth == 1)
			// 	std::cout << "PRUNED SOME LEAVES" << std::endl;
			break;
		}
	}

	results.heuristic = value;
	results.best_move = best_move;
	results.is_finished = is_finished;

	// (* Transposition Table Store; node is the lookup key for tt_entry *)
	set_tt_entry_values(tt_entry, results, alpha_orig, beta, depth);
	// PRINT(tt_entry.best_move);
	if (t_table.contains(node))
		t_table.update(node, tt_entry);
	else
		t_table.insert(node, tt_entry);
		// if (node.get_state() == DEBUG_BOARD.get_state())
		// {
		// 	t_table.lookup(node, tt_entry);
		// 	PRINT("*****!!!!!!!   " << tt_entry.best_move);
		// }
		// why does this improve performance???
		// h_table.insert(node, tt_entry);

	// this slightly reduces amount of visited nodes, but at the cost of table insertions. currently slows down the algo
	// 10-01-2022 why does this increase the amount of visited nodes now??
	
    if (h_table.contains(node)) // apparently, updating is generally bad. but sometimes good. but sometimes bad?
	{
		// std::cout << value << std::endl;
		// node.print();
		// std::cout << h_entry.value << std::endl;
	    h_table.update(node, tt_entry);
		// h_table.lookup(node, h_entry);
		// std::cout << "new value: " << h_entry.value << std::endl;
	}
	else //why does this improve performance????
	{
		h_table.insert(node, tt_entry);
	}
	return results;
}
