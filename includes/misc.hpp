#ifndef MISC_HPP
# define MISC_HPP

#include "Board.hpp"

void					place_pieces(Board &board, int player, int start_pos, int amount, int offset);
int						calc_index(int row, int col);
Board					create_random_board(void);

#endif