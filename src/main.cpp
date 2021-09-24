#include "Board.hpp"
#include "algorithm.hpp"
#include "misc.hpp"
using std::cout;
using std::endl;

int main()
{
    int value;
    int best_value = -std::numeric_limits<int>::max();
    int best_move = -1;
    int first_move_index = 17*19 - 2;
    TranspositionTable t_table;

	// pattern_test(false); /* Only show info */
	// pattern_test(true); /* Show found patterns */
	// heuristic_test();
	// return 0;

    Board node;
    std::vector<int> filled_positions;

    node.place(first_move_index, PLAYER1);
    filled_positions.push_back(node.last_move);
    node.place(first_move_index - 1, PLAYER1);
    filled_positions.push_back(node.last_move);
    // node.place(first_move_index - 2, PLAYER1);
    // filled_positions.push_back(node.last_move);
    node.place(first_move_index - 3, PLAYER1);
    filled_positions.push_back(node.last_move);
    node.place(first_move_index + 1, PLAYER2);
    filled_positions.push_back(node.last_move);
    node.place(first_move_index - 40, PLAYER2);
    filled_positions.push_back(node.last_move);
    node.print();

    std::vector<Board> children = node.generate_children(filled_positions, PLAYER2);
    for (Board child : children)
    {
        // child.print();
        value = -negamax(child, 2, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), PLAYER1, filled_positions, t_table);
        // cout << value << "<-value, move-> " << child.last_move << endl;
		if (value > best_value)
        {
            best_value = value;
            best_move = child.last_move;
        }
    }
    node.place(best_move, PLAYER2);
    node.print();
    std::cout << "best move is: " << best_move << std::endl;

    std::cout << "transposition table size: " << t_table.size() << std::endl;
	std::cout << "times state was found in table: " << FOUND_IN_TABLE << std::endl;
	std::cout << "total branches pruned: " << TOTAL_BRANCHES_PRUNED << std::endl;
    std::cout << "total number of leaves explored: " << TOTAL_LEAVES << std::endl;
    std::cout << "total number of nodes explored: " << TOTAL_NODES << std::endl;

    return 0;
}
