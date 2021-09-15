#ifndef HEURISTIC_HPP
# define HEURISTIC_HPP

# include <cmath>
# include <bitset>
# include <iostream>
# include <cassert>
# include "Board.hpp"

# define HOR 1
# define VER BOARDSIZE
# define DIAG1 (BOARDSIZE + 1)
# define DIAG2 (BOARDSIZE - 1)

typedef struct	s_size
{
	int width;
	int height;
}				t_size;


int						find_pattern(Board state, int player, std::bitset<MASKSIZE> pattern, t_size pattern_size);
void					print_bitmap(std::bitset<MASKSIZE> bitmap);
std::bitset<MASKSIZE>	create_hor(int size, t_size &pattern_size);
std::bitset<MASKSIZE>	create_ver(int size, t_size &pattern_size);
std::bitset<MASKSIZE>	create_diag_left(int size, t_size &pattern_size);
std::bitset<MASKSIZE>	create_diag_right(int size, t_size &pattern_size);

void					pattern_test(void);

#endif
