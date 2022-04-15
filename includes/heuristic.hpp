/* MIT License

Copyright (c) 2022 Flint Louis, Tim de Jong

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#ifndef BOARDHEURISTIC_HPP
# define BOARDHEURISTIC_HPP

class Board;

enum Pattern {none, closed2, open2, closed3, open3, closed4, open4, five};
const char * const PatternNames[8] = {"none", "closed2", "open2", "closed3", "open3", "closed4", "open4", "five"};
const int REMAINING_SCORES[] {0, 100, 900, 1000, 9000, 10000};
const int SCALING_CAPTURE_SCORE = 10;
const int CONSTANT_CAPTURE_SCORE = 200;
const std::map<uint8_t, Pattern> SUBPATTERNS_5 = {	{0b00011111, five},
													{0b00001111, closed4}, {0b00011110, closed4}, {0b00010111, closed4}, {0b00011011, closed4}, {0b00011101, closed4},
													{0b00001110, open3}	 };

const std::map<uint8_t, Pattern> SUBPATTERNS_6 = {	{0b00011110, open4},
													{0b00011010, open3}, {0b00010110, open3}  };

typedef struct s_pattern
{
	uint8_t left_right[2];
	uint8_t count;
	uint8_t space;
    uint8_t length;
    uint8_t pattern = 0;
}   t_pattern;

const int WINNING_POINTS[] = {1000000000, 100000000, 10000000, 1000000};

namespace heuristic
{
	int					count_both_dir(const Board &board, int index, int player, int dir);
	int					count_both_dir(const Board &board, int index, int player, int dir, std::bitset<BOARDSIZE> &checked_indices);
	int					count_direction(const Board &board, int index, int player, int dir, int size, std::bitset<BOARDSIZE> &checked_indices);
	int					count_direction(const Board &board, int index, int player, int dir, int size);
	int					calc_heuristic(Board &board);
	int					eight_directions_heuristic(Board &board, int index, std::bitset<BOARDSIZE> &checked_indices, int player);
	int					determine_score(int count, int gaps, int open);
	int					get_direction(const Board &board, int move, int direction, int player);
	void           		cutout_pattern(const Board &board, int move, int direction, int player, t_pattern &pat);
	t_pattern			get_pattern_data(Board &board, int move, int direction, int player, std::bitset<BOARDSIZE> *checked_indices);
	int					score_remaining_patterns(Board const &board, int player);
	int					evaluate_patterns(Board &board, int player);
	Pattern				find_subpattern(t_pattern &pat, uint8_t length, const std::map<uint8_t, Pattern> &map);
	Pattern				get_heuristic_data(Board &board, const int &move, const int &direction, const int &player, std::bitset<BOARDSIZE> *checked_indices);
	void				get_heuristic_single(Board &board, int move, std::bitset<BOARDSIZE> *checked_indices);
	int					get_heuristic_total(Board &board, int player);
	void  				reset_pattern_arrays(Board &board);
}

#endif