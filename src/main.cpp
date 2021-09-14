#include "Board.hpp"
#include "algorithm.hpp"

int main()
{
    int value;
    int best_value = -std::numeric_limits<int>::max();
    int best_move = -1;
    int first_move_index = 19*19 - 1;
    TranspositionTable t_table;

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
        value = negamax(child, 10, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), PLAYER1, filled_positions, t_table);
        if (value > best_value)
        {
            best_value = value;
            best_move = child.last_move;
        }
    }
    std::cout << "best move is: " << best_move << std::endl;

    std::cout << "transposition table size: " << t_table.size() << std::endl;
    std::cout << "total number of leaves visited: " << TOTAL_LEAVES << std::endl;
    std::cout << "total number of nodes stored: " << TOTAL_NODES << std::endl;

    return 0;
}
