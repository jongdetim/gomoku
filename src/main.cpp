#include "Board.hpp"
#include "algorithm.hpp"
// #include "TranspositionTable.hpp"
#include "misc.hpp"

int main()
{
    int value;
    int best_move;
    // int best_value = -std::numeric_limits<int>::max();
    // int best_move = -1;
    int first_move_index = (19 * 19) /2;
    TranspositionTable h_table;

	// pattern_test(false); /* Only show info */
	// pattern_test(true); /* Show found patterns */
	// heuristic_test();
	// return 0;

    Board node;
    std::vector<int> filled_positions;

    // node.place(first_move_index, PLAYER1);
    // filled_positions.push_back(node.last_move);
    // node.place(first_move_index - 1, PLAYER1);
    // filled_positions.push_back(node.last_move);
    // node.place(first_move_index - 2, PLAYER1);
    // filled_positions.push_back(node.last_move);
    // node.place(first_move_index - 1, PLAYER1);
    // filled_positions.push_back(node.last_move);
    // node.place(first_move_index - 2, PLAYER1);
    // filled_positions.push_back(node.last_move);
    // node.place(first_move_index - 3, PLAYER1);
    // filled_positions.push_back(node.last_move);
    // // node.place(first_move_index - 4, PLAYER1);
    // // filled_positions.push_back(node.last_move);
    // node.print();

    // std::vector<Board> children = node.generate_children(filled_positions, PLAYER2);
    // for (Board child : children)
    // {
    //     // child.print();
    //     value = -negamax(child, 2, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), PLAYER1, filled_positions, t_table);
    //     // cout << value << "<-value, move-> " << child.last_move << endl;
	// 	if (value > best_value)
    //     {
    //         best_value = value;
    //         best_move = child.last_move;
    //     }
    // }

	node = create_random_board(3, filled_positions);
    filled_positions.pop_back();
	node.print();

    for (int depth = 1; depth <= 3; depth++)
    {
		TOTAL_LEAVES = 0;
		TOTAL_NODES = 0;
		FOUND_IN_TABLE = 0;
		TOTAL_BRANCHES_PRUNED = 0;
        TranspositionTable t_table;
	
        value = negamax(node, depth, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), PLAYER2, filled_positions, t_table, h_table, true);
        // std::cout << value << std::endl;
        std::cout << "transposition table size: " << t_table.size() << std::endl;
        std::cout << "times state was found in table: " << FOUND_IN_TABLE << std::endl;
        std::cout << "total branches pruned: " << TOTAL_BRANCHES_PRUNED << std::endl;
        std::cout << "total number of leaves explored: " << TOTAL_LEAVES << std::endl;
        std::cout << "total number of nodes explored: " << TOTAL_NODES << std::endl;
        std::cout << "_________________________________________\n" << std::endl;
        std::unordered_map<int, int> map;
    
        // bool has_collision = false;

        // for(size_t buckets = 0; buckets < t_table.t_table.bucket_count(); buckets++)
        // {
        //     // std::cout << t_table.t_table.bucket_size(bucket) << std::endl;
        //     if (t_table.t_table.bucket_size(buckets) > 0)
        //     {
        //         for(auto bucket = t_table.t_table.begin(buckets); bucket != t_table.t_table.end(buckets); bucket++)
        //             std::cout << bucket->first << std::endl;
        //     }
        // }
    }

    // for(size_t bucket = 0; bucket < h_table.t_table.bucket_count(); bucket++)
    //     std::cout << h_table.t_table.bucket_size(bucket) << std::endl;

    // std::bitset<722>  test(std::string("01100000000000000000000100000000000000011000000000000101000001000000000001000110000000000100000000010000000000100000000000000100000000001010101010010101000000000100000000000000000000000100000000000000000000000010000010000010100000000000100000000000100000001010000000000000100000000000000000000110100000000000000010000000000000000010010010000000000000001000000000000000000001011000000000000001010000000000000100010100000000000001010101000000000100001000010000000000000010101010000100001010001000010000000000000000000100100000100000000000000000000000000100100000000000000000000100000000010000000000000000000000000001100000101010000100000000000000000000010000000001001001000000000000000000000100000000000000000000000000000000"));
    // std::cout << std::hash<std::bitset<722>>{}(test) << std::endl;
    // std::bitset<722> test2(std::string("01100000000000000000000100000000000000011000000000000101000001000000000001000110000000000100000000010000000000000000000000000100000000001010101010010101000000000100000000000000000000000100000000000000000000000010000010000010100000000000100000000000100000001010000000000000100000000000000000000110100000000000000010000000000000000010010010000000000000001000000000000000000001011000000000000001010000000000000100010100000000000001010101000000000100001000010000000000000010101010000100001010001000010000000000000000000100100000100000000000000000000000000100100000000000000000000100000000010000000000000000000000000001100000101010000100000000000000000000010000000001001001000000000000000000100100000000000000000000000000000000"));
    // std::cout << std::hash<std::bitset<722>>{}(test2) << std::endl;
    // std::cout << (test.to_string()) << std::endl;
    // std::cout << (test2.to_string()) << std::endl;

    return 0;
}
