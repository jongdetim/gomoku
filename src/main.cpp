# include "gomoku.hpp"
# include "heuristic.hpp"
# include "Board.hpp"
# include "TranspositionTable.hpp"
# include "misc.hpp"
# include "algorithm.hpp"

void    test()
{
    Board board;
    int index = calc_index(8, 8);
    // int star_index = calc_index(3, 16); 

    // create_star(board, star_index, 3, PLAYER1);
    // create_star(board, star_index, 4, PLAYER2);
    
    // board.remove(star_index);

    // board.place(star_index, PLAYER2);
    // std::cout << "captures : " << board.get_player_captures(PLAYER2) << std::endl;

    board.place(index, PLAYER1);
    board.place(index +1, PLAYER2);
    board.place(index +2, PLAYER1);
    board.place(index +3, PLAYER1);
    board.place(index +4, PLAYER2);
    board.place(index +6, PLAYER1);
    board.place(index - 3, PLAYER1);
    // board.place(index - 21, PLAYER2);
    // board.place(index + 2, PLAYER1);
    // #include "heuristic.hpp" 
    // PRINT(board.calc_heuristic());
    // board = create_random_board(8);
    // TranspositionTable t_table;
    // TranspositionTable h_table;

    TIMEOUT_REACHED = false;
    int player = PLAYER2;
    int best_move = -1;
    Timer timer;
    timer.start();
    int last_best_move;

    for (int depth = 1; depth <= 6 && !TIMEOUT_REACHED; depth++)
    {
        
        TranspositionTable h_table;
        TranspositionTable t_table;
        try
        {
            last_best_move = negamax(board, depth, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), player, t_table, h_table, true, timer);
        }
        catch(const char* e)
        {
            TIMEOUT_REACHED = true;
            PRINT("timeout reached during depth: " << depth << ".\nusing previous depth search results:");
            depth -= 1;
            PRINT("best move: " << best_move << " at depth: " << depth);
            print_stats();
            return;
        }
        best_move = last_best_move;
        PRINT(best_move);
        PRINT(last_best_move);

        // PRINCIPAL VARIATION RETRIEVAL:
        board.print_principal_variation(player, depth, t_table);
        print_stats();
    }    
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

    //     // get the pattern
    //     // Pattern result = get_heuristic_data(board, board.get_last_move(), 0, board.get_last_player());
    //     // board.show_last_move();
    //     // get_heuristic_single(board, board.get_last_move());
        
        // int move = board.get_last_move();
        // board.remove(move);
        // board.place(move + 20, 1);
        // board.show_last_move();
        // PRINT(board.get_last_player());
        // PRINT(get_heuristic_total(board));
    //     // if (board.heuristic.patterns[0] < 4)
    //     // {  
    //     //     // board.show_last_move();
    //     //     // PRINT(PatternNames[result]);
    //     // }
    // }

    // see if some sub-pattern is inside the cutout pattern
    // t_pattern sub;
    // sub.pattern = 0b00000001;
    // sub.length = 3;
    
    // if (board.is_game_won())
    //     std::cout << "Won!!!" << std::endl;
    // else
    //     std::cout << "Not Won :(" << std::endl;

}

int     main()
{
    test();
    return 0;
}
