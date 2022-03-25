#ifndef ARGUMENT_PARSER_HPP
# define ARGUMENT_PARSER_HPP

# include "argparse/argparse.hpp"
# include <Board.hpp>
# include <GUI.hpp>

class NegamaxAi;

namespace argument_parser
{
	void						set_options(argparse::ArgumentParser &program);
	void						parse_args(argparse::ArgumentParser &program, int argc, char **argv);
	GUI     	                get_gui(argparse::ArgumentParser &program, NegamaxAi &ai);
}

#endif
