#include "Board.hpp"
#include "argument_parser.hpp"
#include "misc.hpp"
#include "TranspositionTable.hpp"

int                     main(int argc, char **argv)
{
    NegamaxAi nai;
    argparse::ArgumentParser program("Gomoku");

    argument_parser::set_options(program);
    argument_parser::parse_args(program, argc, argv);

    Board::init_zobrist_map();

    GUI gui = argument_parser::get_gui(program, nai);    
    if (program.is_used("--replay"))
    {
        try
        {
            gui.replay(program.get<std::string>("--replay"));
        }
        catch(const char *e)
        {
            std::cerr << e << std::endl;;
            std::cerr << program;
            std::exit(1);
        }
    }
    else
        gui.play(Board());
    
    // test();
    return 0;
}
