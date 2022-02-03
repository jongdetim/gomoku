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
# define ROW5 			1000
# define ROW4 			100
# define ROW3 			10
# define ROW2 			1
# define BOARD_LENGTH	19
# define BOARDSIZE		(BOARD_LENGTH*BOARD_LENGTH)
# define MASKSIZE		((BOARD_LENGTH*BOARD_LENGTH)<<1)
# define MASK_LENGTH	(BOARD_LENGTH<<1)
# define PLAYER1_ID		1
# define PLAYER2_ID		-1
# define PLAYER1_IDX	0
# define PLAYER2_IDX	1
# define BITBOARD		std::bitset<MASKSIZE>

// makes more sense to be enum?
# define EXACT 0
# define UPPERBOUND 2
# define LOWERBOUND 1

const int				POINTS[6]{0,0,ROW2,ROW3,ROW4,ROW5};
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
# include <stdlib.h>
# include <time.h>
# include <math.h>

#endif