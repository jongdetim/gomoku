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

#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

void on_age(int age)
{
  std::cout << "On age: " << age << '\n';
}

void    set_args(int argc, char **argv)
{
//   try
//   {
    po::options_description desc{"Options"};
    desc.add_options()
      ("help,h", "")
      ("size,s", po::value<std::string>()->default_value("big"), "GUI size")
      ("file,f", po::value<std::string>(), "");

    po::variables_map vm;
    po::store(parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
      std::cout << desc << '\n';
//     else if (vm.count("age"))
//       std::cout << "Age*: " << vm["age"].as<int>() << '\n';
//     else if (vm.count("pi"))
//       std::cout << "Pi: " << vm["pi"].as<float>() << '\n';
//   }
//   catch (const po::error &ex)
//   {
//     std::cerr << ex.what() << '\n';
//   }
}

int main(int argc, char **argv)
{

    set_args(argc, argv);

    // RandomAi rai;
    // NegamaxAi nai;

    // Board board;
    
    // GUI gui(&nai);
    // board.play(gui);
}
