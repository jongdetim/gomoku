#include "Board.hpp"
#include "algorithm.hpp"
#include "misc.hpp"

int main()
{
    int value;
    int best_move;
    // int best_value = -std::numeric_limits<int>::max();
    // int best_move = -1;
    int first_move_index = calc_index(9, 15);
    TranspositionTable h_table;

	// pattern_test(false); /* Only show info */
	// pattern_test(true); /* Show found patterns */
	// heuristic_test();
	// return 0;

    Board node;
    std::vector<int> filled_positions;
    // node.place(4, PLAYER2);
    node.place(first_move_index, PLAYER1);
    filled_positions.push_back(first_move_index);
    node.place(first_move_index + 1, PLAYER1);
    filled_positions.push_back(first_move_index+1);
    node.place(first_move_index + 2, PLAYER1);
    filled_positions.push_back(first_move_index+2);
    node.place(first_move_index + 3, PLAYER1);
    filled_positions.push_back(first_move_index+3);
    // node.place(76 + 1, PLAYER1);
    // node.place(100, PLAYER2);
    node.print();

    std::vector<Board> children = node.generate_children(filled_positions, PLAYER2);
    for (int depth = 0; depth <= 3; depth++)
    {
        int best_value = -std::numeric_limits<int>::max();
        best_move = -1;
        FOUND_IN_TABLE = 0;
        TOTAL_BRANCHES_PRUNED = 0;
        TOTAL_LEAVES = 0;
        TOTAL_NODES = 0;
        TranspositionTable t_table;
        // auto comp = [&](Board a, Board b)-> bool
        // {
        //     // std::cout << "last_move: " << node.last_move << std::endl;
        //         return a.h < b.h;
        // };

        // // calculate heuristic for all the children to sort by. using lambda as comparison function to pass color param
        // if (depth > 0)
        // {
        //     for (Board &child : children)
        //     {
        //         TableEntry tt_entry_lower;
        //         if (h_table.lookup(child, tt_entry_lower))
        //         {
        //             std::cout << "al gezien" << std::endl;
        //             child.h = PLAYER2 * tt_entry_lower.value;
        //         }
        //         else
        //         {
        //             std::cout << "calculating child h" << std::endl;
        //             child.h = calc_heuristic(child);
        //         }
        //     }
        //     std::sort(children.begin(), children.end(), comp);
        // }
        for (Board child : children)
        {
            // child.print();
            value = -negamax(child, depth, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), PLAYER1, filled_positions, t_table, h_table);
            TableEntry tt_entry;
            tt_entry.value = value;
            h_table.insert(child, tt_entry);
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
    }
    

    // std::cout << "transposition table size: " << t_table.size() << std::endl;
	// std::cout << "times state was found in table: " << FOUND_IN_TABLE << std::endl;
	// std::cout << "total branches pruned: " << TOTAL_BRANCHES_PRUNED << std::endl;
    // std::cout << "total number of leaves explored: " << TOTAL_LEAVES << std::endl;
    // std::cout << "total number of nodes explored: " << TOTAL_NODES << std::endl;
    node.place(best_move, PLAYER2);
    node.print();

    return 0;
}
