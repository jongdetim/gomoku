#include "argument_parser.hpp"

void                    argument_parser::set_options(argparse::ArgumentParser &program)
{
    program.add_argument("--size", "-s")
    .default_value("big")
    .required()
    .help("GUI size (big|medium|small)");

    program.add_argument("--replay", "-r")
    .help("Replay game from file [filename format: (id).board.data]");
}

void					argument_parser::parse_args(argparse::ArgumentParser &program, int argc, char **argv)
{
    try
    {
        program.parse_args(argc, argv);
        
        if (program.is_used("--size"))
        {
            auto size = program.get<std::string>("--size");
            if (size != "small" || size != "medium" || size != "big")
                throw "--size: no acceptable value provided.";
        }
    }
    catch (const std::runtime_error& err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }
    catch (char const* &str)
    {
        std::cerr << str << '\n';
        std::cerr << program;
        exit(1);
    }
}

GUI                     argument_parser::get_gui(argparse::ArgumentParser &program, NegamaxAi &ai)
{
    std::string size;
    
    if (program.is_used("--size"))
    {
        auto size = program.get<std::string>("--size");
        return (size == "small") ? GUI(&ai, small) : (size == "medium") ? GUI(&ai, medium) : GUI(&ai, big);
    }
    return GUI(&ai);
}
