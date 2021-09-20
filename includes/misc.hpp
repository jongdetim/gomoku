#ifndef MISC_HPP
# define MISC_HPP

#include "Board.hpp"

# define HOR 1
# define VER BOARDSIZE
# define DIAG1 (BOARDSIZE + 1)
# define DIAG2 (BOARDSIZE - 1)

void					place_pieces(Board &board, int player, int start_pos, int amount, int offset);
int						calc_index(int row, int col);
Board					create_random_board(void);
void					pattern_test(bool verbose);
void					test_board_class(void);
void					heuristic_test(void);

#endif
