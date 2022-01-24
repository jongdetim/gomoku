#ifndef MISC_HPP
# define MISC_HPP

# include "gomoku.hpp"
# include "Board.hpp"

#define PRINT(x) std::cout << x << std::endl

Board					create_random_board(void);
Board					create_random_board(int seed);
int						get_col(int index);
int						get_row(int index);
int						calc_index(int row, int col);
bool					is_offside(int prev_index, int index);
void					create_star(Board &board, int index, int size, int player);
void					place_pieces(Board &board, int player, int start_pos, int amount, int offset);
void                    print_and_quit(const char *msg);

#endif
