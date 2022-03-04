#ifndef ARGUMENT_PARSER_HPP
# define ARGUMENT_PARSER_HPP

# include <boost/program_options.hpp>
# include <Board.hpp>
# include <GUI.hpp>

namespace po = boost::program_options;

namespace argument_parser
{
	po::options_description		get_options(void);
	po::variables_map       	get_args(int argc, char **argv, po::options_description &options);
	Board						get_board(po::variables_map &vm);
	GUI     	                get_gui(po::variables_map &vm, IAi &ai);
}

#endif
