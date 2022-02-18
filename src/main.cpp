#include "Board.hpp"
#include "GUI.hpp"
#include "PrimitiveGui.hpp"
#include "gomoku.hpp"
#include "heuristic.hpp"
#include "TranspositionTable.hpp"
#include "misc.hpp"
#include "algorithm.hpp"

class RandomAi : public IAi
{
public:
	int	calculate(Board &board)
    {
        return misc::get_random_int_range(0, (BOARDSIZE - 1));
    }
};

class NegamaxAi : public IAi
{
public:
	int	calculate(Board &board)
    {
        if (board.is_empty())
            return misc::calc_index(8,9);

        board.next_player(); // Need to find a neater solution for this. Segfaults if using current_player
        int move = this->iterative_deepening_negamax(board, board.get_next_player());
        board.next_player(); // Need to find a neater solution for this. Segfaults if using current_player
        return move;
    }
private:
    int iterative_deepening_negamax(Board &board, int player)
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
                return best_move;
            }
            best_move = last_best_move;
        }   
        return -1;
    }
};

e_gui_size parse_arguments(int argc, char **argv)
{
    if (argc == 1)
        return big;
    
    std::string str(argv[1]);
    if (str == "small")
        return small;
    if (str == "medium")
        return medium;
    return big;
}

int main(int argc, char **argv)
{
    // RandomAi ai;
    NegamaxAi ai;
    GUI gui(&ai, parse_arguments(argc, argv));
    Board board = misc::create_random_board(0);

    // board.print();
    board.play(gui);
}
