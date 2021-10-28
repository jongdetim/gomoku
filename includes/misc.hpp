#ifndef MISC_HPP
# define MISC_HPP

#include "Board.hpp"
#include <stdlib.h>
#include <cassert>
#include <time.h>
#include <vector>

# define HOR 1
# define VER BOARD_LENGHT
# define DIAG1 (BOARD_LENGHT + 1)
# define DIAG2 (BOARD_LENGHT - 1)

int						get_col(int index);
int						get_row(int index);
void					place_pieces(Board &board, int player, int start_pos, int amount, int offset);
void					place_pieces(Board &board, int player, int start_pos, int amount, int offset, std::vector<int> &filled_positions);
int						calc_index(int row, int col);
Board					create_random_board(std::vector<int> &filled_positions);
Board					create_random_board(int seed, std::vector<int> &filled_positions);
void					pattern_test(bool verbose);
void					test_board_class(void);
void					heuristic_test(void);
int	    				play_random_game(Board board, int player, bool verbose);
int	    				play_random_game(Board board, int player, std::vector<int> empty, bool verbose);
int						play_random_games(Board &board, int start_player, int amount, bool verbose);
int						play_random_games(Board &board, int start_player, int amount, bool verbose, int seed);

#endif
