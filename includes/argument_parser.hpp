#ifndef ARGUMENT_PARSER_HPP
# define ARGUMENT_PARSER_HPP

# include <boost/program_options.hpp>
# include <Board.hpp>
# include <GUI.hpp>

namespace po = boost::program_options;

class NegamaxAi;

namespace argument_parser
{
	std::string             	get_file(po::variables_map &vm);
	po::options_description		get_options(void);
	po::variables_map       	get_args(int argc, char **argv, po::options_description &options);
	GUI     	                get_gui(po::variables_map &vm, NegamaxAi &ai);
}

#endif
