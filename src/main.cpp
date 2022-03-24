#include "Board.hpp"
#include "argument_parser.hpp"
#include "misc.hpp"
#include "TranspositionTable.hpp"

void    iterative_deepening_negamax(Board &board, int player)
{
    TIMEOUT_REACHED = false;
    Timer timer;
    int depth = 1;
    int max_depth = 7;

    t_search_results results;
	t_search_results last_results;
    TranspositionTable h_table;
    TranspositionTable t_table;

    timer.start();

    for (; depth <= max_depth && !TIMEOUT_REACHED; depth++)
    {
        
        // TranspositionTable h_table;
        // TranspositionTable t_table;
        try
        {
            last_results = negamax(board, depth, depth, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), player, t_table, h_table, true, timer);
        }
        catch(const char* e)
        {
            TIMEOUT_REACHED = true;
            PRINT("timeout reached during depth: " << depth << ".\nusing previous depth search results:");
            depth -= 1;
            PRINT("best move: " << misc::get_col(results.best_move) << ", " << misc::get_row(results.best_move) << " at depth: " << depth);
            misc::print_stats();
            return;
        }
        results = last_results;
        PRINT("best move: " << misc::get_col(results.best_move) << ", " << misc::get_row(results.best_move) << " at depth: " << depth);

        Board node = board;
        node.place(results.best_move, player);
        node.show_last_move();
        // PRINCIPAL VARIATION RETRIEVAL:
        board.print_principal_variation(player, depth, t_table);
        misc::print_stats();

        TableEntry tt_entry;
        t_table.retrieve(board, tt_entry);
        PRINT(tt_entry.depth);
        PRINT(tt_entry.results.best_move);

        if (results.is_finished)
            break;
    }    
}

// class RandomAi : public IAi
// {
// public:
// 	int	calculate(Board &board)
//     {
//         return misc::get_random_int_range(0, (BOARDSIZE - 1));
//     }
// };

// class NegamaxAi : public IAi
// {
// public:
// 	int	calculate(Board &board)
//     {
//         if (board.is_empty())
//             return misc::calc_index(9,9);

//         int move = this->iterative_deepening_negamax(board, board.get_current_player());
//         return move;
//     }
// private:
//     int iterative_deepening_negamax(Board board, int player)
//     {
//         TIMEOUT_REACHED = false;
//         int best_move = -1;
//         Timer timer;
//         int last_best_move;
//         int depth = 1;
//         int max_depth = 7;

//         timer.start();
//         // TranspositionTable last_t_table;
//         TranspositionTable h_table;
//         TranspositionTable t_table;
//         for (; depth <= max_depth && !TIMEOUT_REACHED; depth++)
//         {
            
//             // TranspositionTable h_table;
//             // TranspositionTable t_table;
//             try
//             {
//                 last_best_move = negamax(board, depth, depth, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), player, t_table, h_table, true, timer);
//             }
//             catch(const char* e)
//             {   
//                 PRINT("depth: " << depth - 1);
//                 // board.print_principal_variation(player, depth - 1, t_table);
//                 PRINT("timeout reached during depth: " << depth << ".\nusing previous depth search results:");
//                 board.place(best_move);
//                 TableEntry tt_entry;
//                 // t_table.retrieve(board, tt_entry);
//                 PRINT("heuristic: " << tt_entry.value);
//                 return best_move;
//             }
//             best_move = last_best_move;
//             // last_t_table = t_table;
//             misc::print_stats();
//         }
//         return best_move;
//     }
// };

void    test()
{
    Board board;

    int index = misc::calc_index(9, 9);
    board.place(index, PLAYER1);
    // board.place(index + 1, PLAYER1);
    // board.place(index + 2, PLAYER1);
    // board.place(index - 20, PLAYER2);
    // board.place(index - 38, PLAYER1);

    // board.place(index +1, PLAYER2);
    // board.place(index +2, PLAYER1);
    // board.place(index +3, PLAYER1);
    // board.place(index +4, PLAYER2);
    // board.place(index +6, PLAYER1);
    // board.place(index - 3, PLAYER1);
    // board.print();

    // PRINT(heuristic::get_heuristic_total(board));

    // DEBUG_BOARD.place(140, PLAYER1);
    // DEBUG_BOARD.place(160, PLAYER1);
    // DEBUG_BOARD.place(180, PLAYER1);
    // DEBUG_BOARD.place(120, PLAYER2);
    // DEBUG_BOARD.place(200, PLAYER2);
    // DEBUG_BOARD.place(141, PLAYER2);

    int player = PLAYER2;
    iterative_deepening_negamax(board, player);

    // NegamaxAi ai;
    // board.set_current_player(player);
    // ai.calculate(board);

    // board.place(index - 21, PLAYER2);
    // board.place(index + 2, PLAYER1);
    // #include "heuristic.hpp" 
    // PRINT(board.calc_heuristic());
    // board = create_random_board(8);
    // TranspositionTable t_table;
    // TranspositionTable h_table;

    
    // PRINT(TOTAL_NODES);
    // PRINT(TOTAL_LEAVES);
    // PRINT(FOUND_IN_TABLE);
    // board.place(360, PLAYER1);
    // Pattern result;
    // board = create_random_board(1);
    // for (int i = 0; i < 230000; i++)
    // {
    //     // ooooo.o.o. -> 000.0.0. would be solved by using 16 bits. what would be the performance impact?
    //     // board = create_random_board(1);
}

void init_zobrist_map()
{
    std::random_device rd;
    std::mt19937_64 gen(rd()); //Using the 64-bit Mersenne Twister 19937 generator
    std::uniform_int_distribution<unsigned long long> distrib;

    for (int i = 0; i < MASKSIZE; i++)
		Board::zobrist_map[i] = distrib(gen);
}

int                     main(int argc, char **argv)
{
    NegamaxAi nai;

    po::options_description options = argument_parser::get_options();
    po::variables_map vm = argument_parser::get_args(argc, argv, options);
    
    if (vm.count("help"))
    {
        std::cout << options << '\n';
        exit(0);
    }

    init_zobrist_map();
    
    GUI gui = argument_parser::get_gui(vm, nai);
    
    if (vm.count("replay"))
    {
        try
        {
            gui.replay(argument_parser::get_file(vm));
        }
        catch(const char *e)
        {
            std::cerr << e << '\n';
            exit(1);
        }
    }
    else
        gui.play(Board());
    // test();
    return 0;
}
