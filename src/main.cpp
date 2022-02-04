#include "Board.hpp"
#include "GUI.hpp"
#include "PrimitiveGui.hpp"
#include "gomoku.hpp"
#include "heuristic.hpp"
#include "TranspositionTable.hpp"
#include "misc.hpp"
#include "algorithm.hpp"

void    iterative_deepening_negamax(Board &board, int player)
{
    TIMEOUT_REACHED = false;
    int best_move = -1;
    Timer timer;
    timer.start();
    int last_best_move;
    int depth = 1;
    int max_depth = 7;

    for (; depth <= max_depth && !TIMEOUT_REACHED; depth++)
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
            PRINT("best move: " << misc::get_col(best_move) << ", " << misc::get_row(best_move) << " at depth: " << depth);
            misc::print_stats();
            return;
        }
        best_move = last_best_move;
        PRINT("best move: " << misc::get_col(best_move) << ", " << misc::get_row(best_move) << " at depth: " << depth);
        // PRINT(last_best_move);

        Board node = board;
        node.place(best_move, player);
        node.show_last_move();
        // PRINCIPAL VARIATION RETRIEVAL:
        board.print_principal_variation(player, depth, t_table);
        misc::print_stats();
    }    
}

class RandomAi : public IAi
{
public:
	int	calculate(Board &board)
    {
        return misc::random(0, (BOARDSIZE - 1));
    }
};

// int main()
// {
//     // PrimitiveGui gui(NULL);
//     RandomAi ai;
//     GUI gui(&ai, medium);

//     Board board;
    
//     board.play(gui);
// }

void    test()
{
    Board board;

    int index = misc::calc_index(8, 8);
    board.place(index, PLAYER1);
    // board.place(index - 20, PLAYER2);
    // board.place(index - 38, PLAYER1);

    // board.place(index +1, PLAYER2);
    // board.place(index +2, PLAYER1);
    // board.place(index +3, PLAYER1);
    // board.place(index +4, PLAYER2);
    // board.place(index +6, PLAYER1);
    // board.place(index - 3, PLAYER1);
    // board.print();

    // PRINT(Heuristic::get_heuristic_total(board));

    int player = PLAYER2;
    iterative_deepening_negamax(board, player);
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
