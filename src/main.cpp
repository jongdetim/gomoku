#include "Board.hpp"
#include "GUI.hpp"
#include "PrimitiveGui.hpp"
#include "gomoku.hpp"
#include "heuristic.hpp"
#include "TranspositionTable.hpp"
#include "misc.hpp"
#include "algorithm.hpp"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

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

po::options_description get_options(void)
{
    po::options_description options{"Options"};
    options.add_options()
    ("help,h", "")
    ("size,s", po::value<std::string>()->default_value("big"), "GUI size [ big | medium | small ]")
    ("file,f", po::value<std::string>(), "Load board state from file");
    return options;
}

po::variables_map       get_args(int argc, char **argv, po::options_description &options)
{
    po::variables_map vm;
    try
    {
        po::store(po::parse_command_line(argc, argv, options), vm);
        po::notify(vm);
    }
    catch (const po::error &e)
    {
        std::cerr << e.what() << '\n';
        exit(1);
    }
    return vm;
}

Board                   get_board(po::variables_map &vm)
{
    Board board;
    std::string file;

    try
    {
        if (vm.count("file"))
        {
            file = vm["file"].as<std::string>();
            board.load(file);
        }
    }
    catch (char const* &str)
    {
        std::cerr << str << '\n';
        exit(1);
    }
    return board;
}

GUI                     get_gui(po::variables_map &vm)
{
    NegamaxAi nai;
    std::string size;
    
    try
    {
        if (vm.count("size"))
        {
            size = vm["size"].as<std::string>();
            if (!(size == "small" || size == "medium" || size == "big"))
                throw po::validation_error(po::validation_error::invalid_option_value, "size");
            return (size == "small") ? GUI(&nai, small) : (size == "medium") ? GUI(&nai, medium) : GUI(&nai, big);
        }
    }
    catch (const po::error &e)
    {
        std::cerr << e.what() << '\n';
        exit(1);
    }
    return GUI(&nai);
}

int main(int argc, char **argv)
{
    po::options_description options = get_options();
    po::variables_map vm = get_args(argc, argv, options);

    if (vm.count("help"))
    {
        std::cout << options << '\n';
        exit(0);
    }

    GUI gui = get_gui(vm);
    Board board = get_board(vm);
    board.play(gui);
}
