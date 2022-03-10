#include "Board.hpp"
#include "argument_parser.hpp"
#include "algorithm.hpp"

int                     main(int argc, char **argv)
{
    po::options_description options = argument_parser::get_options();
    po::variables_map vm = argument_parser::get_args(argc, argv, options);

    if (vm.count("help"))
    {
        std::cout << options << '\n';
        exit(0);
    }

    NegamaxAi nai;
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
}
