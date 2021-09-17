#include "Board.hpp"
#include "algorithm.hpp"
#include "misc.hpp"

int main()
{
    int value;
    int best_value = -std::numeric_limits<int>::max();
    int best_move = -1;
    int first_move_index = 19*19 - 1;
    TranspositionTable t_table;

	// pattern_test(false); /* Only show info */
	// pattern_test(true); /* Show found patterns */
	// heuristic_test();
	// return 0;

    Board node;
    node.place(first_move_index, PLAYER1);
    // node.place(100, PLAYER2);
    node.print();

    std::vector<int> filled_positions;
    filled_positions.push_back(node.last_move);
    std::vector<Board> children = node.generate_children(filled_positions, PLAYER2);
    for (Board child : children)
    {
        // child.print();
        value = negamax(child, 5, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), PLAYER1, filled_positions, t_table);
		if (value > best_value)
        {
            best_value = value;
            best_move = child.last_move;
        }
    }
    std::cout << "best move is: " << best_move << std::endl;

    std::cout << "transposition table size: " << t_table.size() << std::endl;
	std::cout << "times state was found in table: " << FOUND_IN_TABLE << std::endl;
	std::cout << "total branches pruned: " << TOTAL_BRANCHES_PRUNED << std::endl;
    std::cout << "total number of leaves explored: " << TOTAL_LEAVES << std::endl;
    std::cout << "total number of nodes explored: " << TOTAL_NODES << std::endl;

    return 0;
}
