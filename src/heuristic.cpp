#include "heuristic.hpp"
#include "misc.hpp"

// #define PRINT(x) std::cout << x << std::endl
// #define PRINTENDL std::cout << std::endl

int						get_hor_pattern(Board &board, int start_index, int player)
{
	int pattern = 0;
	int col = get_col(start_index);
	int leftside = col < 4 ?  col : 4;
	int offset = (BOARD_LENGHT-1) - col;
	int rightside = offset < 4 ?  offset : 4;
	int total = leftside + rightside + 1;
	int index = start_index - leftside;

	for (int i = 0; i < total; i++)
	{
		if (board.get_player(index) == -player)
		{
			if (index < start_index)
				pattern = 0;
			else
				break ;
		}
		else if (board.get_player(index) == player)
			pattern = (pattern << 1) ^ 1;
		else
			pattern <<= 1;
		index++;
	}
	return pattern;
}

static int				go_down(Board &board, int index, int player)
{
	int length = 0;

	for (int i = 0; i < 4; i++)
	{
		index += BOARD_LENGHT;
		if (index >= BOARDSIZE or board.get_player(index) != player)
			break ;
		length++;
	}
	return length;
}

static int				go_up(Board &board, int index, int player)
{
	int length = 0;

	for (int i = 0; i < 4; i++)
	{
		index -= BOARD_LENGHT;
		if (index < 0 or board.get_player(index) != player)
			break ;
		length++;
	}
	return length;
}

int						count_ver(Board &board, int index, int player)
{
	int total = 1;

	total += go_up(board, index, player);
	total += go_down(board, index, player);
	return total;
}

static int				go_left(Board &board, int index, int player)
{
	int length = 0;
	int col = get_col(index);
	int offside = col < 4 ?  col : 4;

	for (int i = 0; i < offside; i++)
	{
		index--;
		if (board.get_player(index) != player)
			break ;
		length++;
		
	}
	return length;
}

static int				go_right(Board &board, int index, int player)
{
	int length = 0;
	int offset = (BOARD_LENGHT-1) - get_col(index);
	int offside = offset < 4 ?  offset : 4;

	for (int i = 0; i < offside; i++)
	{
		index++;
		if (board.get_player(index) != player)
			break ;
		length++;
		
	}
	return length;
}

int						count_hor(Board &board, int index, int player)
{
	int total = 1;

	total += go_left(board, index, player);
	total += go_right(board, index, player);
	return total;
}

static int				diag_upR(Board &board, int index, int player)
{
	int length = 0;
	int offset = (BOARD_LENGHT-1) - get_col(index);
	int offside = offset < 4 ?  offset : 4;

	for (int i = 0; i < offside; i++)
	{
		index -= (BOARD_LENGHT -1);
		if (index < 0 or board.get_player(index) != player)
			break ;
		length++;
		
	}
	return length;
}

static int				diag_downL(Board &board, int index, int player)
{
	int length = 0;
	int col = get_col(index);
	int offside = col < 4 ?  col : 4;

	for (int i = 0; i < offside; i++)
	{
		index += (BOARD_LENGHT-1);
		if (index >= BOARDSIZE or board.get_player(index) != player)
			break ;
		length++;
		
	}
	return length;
}

int						count_diagR(Board &board, int index, int player)
{
	int total = 1;

	total += diag_upR(board, index, player);
	total += diag_downL(board, index, player);
	return total;
}

static int				diag_upL(Board &board, int index, int player)
{
	int length = 0;
	int col = get_col(index);
	int offside = col < 4 ?  col : 4;

	for (int i = 0; i < offside; i++)
	{
		index -= (BOARD_LENGHT+1);
		if (index < 0 or board.get_player(index) != player)
			break ;
		
		length++;
	}
	return length;
}

static int				diag_downR(Board &board, int index, int player)
{
	int length = 0;
	int offset = (BOARD_LENGHT-1) - get_col(index);
	int offside = offset < 4 ?  offset : 4;

	for (int i = 0; i < offside; i++)
	{
		index += (BOARD_LENGHT+1);
		if (index >= BOARDSIZE or board.get_player(index) != player)
			break ;
		
		length++;
	}
	return length;
}

int						count_diagL(Board &board, int index, int player)
{
	int total = 1;

	total += diag_upL(board, index, player);
	total += diag_downR(board, index, player);
	return total;
}

BITBOARD				create_ver(int size, t_size &pattern_size)
{
	BITBOARD pattern{1};
	pattern_size.height = size; pattern_size.width = 1;

	for (int i = 1; i < size; i++)
	{
		pattern <<= MASK_LENGTH;
		pattern ^= 1;
	}
	return pattern;
}

BITBOARD				create_hor(int size, t_size &pattern_size)
{
	BITBOARD pattern{1};
	pattern_size.height = 1; pattern_size.width = size;

	for (int i = 1; i < size; i++)
	{
		pattern <<= 2;
		pattern ^= 1;
	}
		
	return pattern;
}

BITBOARD				create_diag_left(int size, t_size &pattern_size)
{
	BITBOARD pattern{1};
	pattern_size.height = size; pattern_size.width = size;

	for (int i = 1; i < size; i++)
	{
		pattern <<= (MASK_LENGTH + 2);
		pattern ^= 1;
	}
		
	return pattern;
}

BITBOARD				create_diag_right(int size, t_size &pattern_size)
{
	BITBOARD pattern{1};
	pattern_size.height = size; pattern_size.width = size;

	for (int i = 1; i < size; i++)
	{
		pattern <<= MASK_LENGTH;
		pattern ^= (1 << (i << 1));
	}
		
	return pattern;
}

static inline bool		is_out_of_bounds(int i, int pattern_height)
{
	return ((i / MASK_LENGTH) + pattern_height) > BOARD_LENGHT;
}

static inline bool 		is_at_edge(int i, int pattern_width)
{
	return (i != 0 and (i + (pattern_width)) % MASK_LENGTH == 0);
}

static inline bool		pattern_found(Board state, BITBOARD pattern)
{
	return ((state & pattern) == pattern);
}

int						find_pattern(Board &state, int player, BITBOARD &pattern, t_size pattern_size, bool verbose)
{
	assert(pattern_size.width <= BOARD_LENGHT and pattern_size.height <= BOARD_LENGHT);
	pattern_size.width <<= 1;
	int found = 0;
	pattern = player == PLAYER1 ? pattern : pattern << 1;
	for (int i = 0; i < MASKSIZE; i+=2)
	{
		if (is_out_of_bounds(i, pattern_size.height))
			break ;
		if(pattern_found(state, (pattern<<i)))
		{
			if (verbose)
				print_bitmap(pattern<<i);
			found++;
		}
		if (is_at_edge(i, pattern_size.width))
			i += (pattern_size.width - 2);
	}
	return found;
}

inline int				get_end_bit(BITBOARD &pattern, t_size pattern_size)
{
	return (pattern[(pattern_size.width * pattern_size.height) - 1] == false);
}

int						g_points[5]{0,ROW2,ROW3,ROW4,ROW5}; // Just to test

int						get_heuristic(Board &state, BITBOARD &pattern, t_size pattern_size)
{
	assert(pattern_size.width <= BOARD_LENGHT and pattern_size.height <= BOARD_LENGHT);
	int points = g_points[std::max(pattern_size.width, pattern_size.height) - 1];
	int score = 0;
	int player = PLAYER1;
	pattern_size.width <<= 1;
	pattern_size.width -= get_end_bit(pattern, pattern_size);
	for (int i = 0; i < MASKSIZE; i++)
	{
		if (is_out_of_bounds(i, pattern_size.height))
			break ;
		if(pattern_found(state, (pattern<<i)))
			score += player * points;
		if (is_at_edge(i, pattern_size.width))
			i += (pattern_size.width - 1);
		player = -player;
	}
	return score;
}

int						calc_heuristic(Board &state)
{
	int						sizes[4]{2,3,4,5};
	t_size					size_pattern;
	BITBOARD	pattern;
	int						score = 0;

	for (auto size : sizes)
	{
		pattern = create_ver(size, size_pattern);
		score += get_heuristic(state, pattern, size_pattern);

		pattern = create_hor(size, size_pattern);
		score += get_heuristic(state, pattern, size_pattern);

		pattern = create_diag_left(size, size_pattern);
		score += get_heuristic(state, pattern, size_pattern);

		pattern = create_diag_right(size, size_pattern);
		score += get_heuristic(state, pattern, size_pattern);
	}
	return score;
}
