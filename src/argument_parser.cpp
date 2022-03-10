#include "argument_parser.hpp"

po::options_description argument_parser::get_options(void)
{
    po::options_description options{"Options"};
    options.add_options()
    ("help,h", "")
    ("size,s", po::value<std::string>()->default_value("big"), "GUI size [ big | medium | small ]")
    ("replay,r", po::value<std::string>(), "Replay game from file [ <id>.board.data ]");
    return options;
}

po::variables_map       argument_parser::get_args(int argc, char **argv, po::options_description &options)
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

std::string             argument_parser::get_file(po::variables_map &vm)
{
    try
    {
        return vm["replay"].as<std::string>();
    }
    catch (char const* &str)
    {
        std::cerr << str << '\n';
        exit(1);
    }
}

GUI                     argument_parser::get_gui(po::variables_map &vm, IAi &ai)
{
    std::string size;
    
    try
    {
        if (vm.count("size"))
        {
            size = vm["size"].as<std::string>();
            if (!(size == "small" || size == "medium" || size == "big"))
                throw po::validation_error(po::validation_error::invalid_option_value, "size");
            return (size == "small") ? GUI(&ai, small) : (size == "medium") ? GUI(&ai, medium) : GUI(&ai, big);
        }
    }
    catch (const po::error &e)
    {
        std::cerr << e.what() << '\n';
        exit(1);
    }
    return GUI(&ai);
}
