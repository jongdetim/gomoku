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
#ifndef GOMOKU_HPP
# define GOMOKU_HPP

# define WINCONDITION	5
# define CAPTUREWIN		5
# define LEFT			-1
# define RIGHT			1
# define UP				-(BOARD_LENGTH)
# define DOWN			BOARD_LENGTH
# define DIAGUPR		-(BOARD_LENGTH - 1)
# define DIAGUPL		-(BOARD_LENGTH + 1)
# define DIAGDWNR		(BOARD_LENGTH + 1)
# define DIAGDWNL		(BOARD_LENGTH - 1)
# define BOARD_LENGTH	19
# define BOARDSIZE		(BOARD_LENGTH*BOARD_LENGTH)
# define MASKSIZE		((BOARD_LENGTH*BOARD_LENGTH)<<1)
# define MASK_LENGTH	(BOARD_LENGTH<<1)
# define PLAYER1		0
# define PLAYER2		1
# define BITBOARD		std::bitset<MASKSIZE>

// makes more sense to be enum?
# define EXACT 0
# define UPPERBOUND 2
# define LOWERBOUND 1

const int				DIRECTIONS[8] = {DIAGUPL, UP, DIAGUPR, RIGHT, DIAGDWNR, DOWN, DIAGDWNL, LEFT};

# include <iostream>
# include <cassert>
# include <bitset>
# include <vector>
# include <unordered_set>
# include <set>
# include <algorithm>
# include <unordered_map>
# include <map>
# include <random>
# include <stdlib.h>
# include <time.h>
# include <math.h>

#endif