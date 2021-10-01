#ifndef HEURISTIC_HPP
# define HEURISTIC_HPP

# include <cmath>
# include <bitset>
# include <iostream>
# include <cassert>
# include "Board.hpp"

# define ROW5 100000
# define ROW4 15000
# define ROW3 10000
# define ROW2 5000

typedef struct	s_size
{
	int width;
	int height;
}				t_size;

int						calc_heuristic(Board &state);
int						get_heuristic(Board &state, BITBOARD &pattern, t_size pattern_size);
int						find_pattern(Board &state, int player, BITBOARD &pattern, t_size pattern_size, bool verbose);
void					print_bitmap(BITBOARD bitmap);
BITBOARD				create_hor(int size, t_size &pattern_size);
BITBOARD				create_ver(int size, t_size &pattern_size);
BITBOARD				create_diag_left(int size, t_size &pattern_size);
BITBOARD				create_diag_right(int size, t_size &pattern_size);

#endif
