#ifndef MISC_HPP
# define MISC_HPP

# include "gomoku.hpp"
# include "Board.hpp"

namespace misc
{
	Board						create_random_board(void);
	Board						create_random_board(int seed);
	int							get_col(int index);
	int							get_row(int index);
	int							calc_index(int row, int col);
	bool						is_offside(int prev_index, int index);
	void						create_star(Board &board, int index, int size, int player_index);
	void						place_pieces(Board &board, int start_pos, int amount, int offset);
	int							random(int min, int max);
	int							random(int min, int max, int seed);
	int							random_int(void);
	std::vector<std::string>	tokenize(std::string &str, char delim);
}

#endif
