#include "misc.hpp"
#include "heuristic.hpp"
#include "BoardHeuristic.hpp"

int					get_col(int index)
{
	return (index % BOARD_LENGTH);
}

int					get_row(int index)
{
	return (index / BOARD_LENGTH);
}

int					calc_index(int row, int col)
{
	return (row * BOARD_LENGTH + col);
}

void    			print_bitboard(BITBOARD bitmap)
{
	std::cout << std::endl << "   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8" << std::endl;
	for (int row = 0; row < BOARD_LENGTH; row++)
	{
		std::cout << row%10 << ": ";
		for (int col = 0; col < BOARD_LENGTH; col++)
		{
			int index = (row * BOARD_LENGTH + col) << 1;
			if (bitmap[index])
				std::cout << 'o' << ' ';
			else if (bitmap[index + 1])
				std::cout << 'x' << ' ';
			else
				std::cout << ". ";
		}
		std::cout << std::endl;
	}
}

static bool			is_offside(int prev_index, int index)
{
	int row = get_row(index), col = get_col(index);
	int prev_row = get_row(prev_index), prev_col = get_col(prev_index);

	return ((abs(prev_row - row) > 1) or (abs(prev_col - col) > 1));
}

void				place_pieces(Board &board, int player, int start_pos, int amount, int offset, std::vector<int> &filled_positions)
{
	int index = start_pos;
	int prev_index;

	assert(start_pos >= 0 and start_pos < (BOARD_LENGTH*BOARD_LENGTH));
	for (int i = 1; i <= amount; i++)
	{
		if (index >= (BOARD_LENGTH*BOARD_LENGTH) or index < 0)
			break ;
		// if (!board.is_empty_place(index))
		// 	continue ;
		board.place(index, player);
		filled_positions.push_back(index);
		prev_index = index;
		index = start_pos + (i * offset);
		if (is_offside(prev_index, index))
			break ;
		
	}
}

void				place_pieces(Board &board, int player, int start_pos, int amount, int offset)
{
	int index = start_pos;
	int prev_index;

	assert(start_pos >= 0 and start_pos < (BOARD_LENGTH*BOARD_LENGTH));
	for (int i = 1; i <= amount; i++)
	{
		if (index >= (BOARD_LENGTH*BOARD_LENGTH) or index < 0)
			break ;
		// if (!board.is_empty_place(index))
		// 	continue ;
		board.place(index, player);
		prev_index = index;
		index = start_pos + (i * offset);
		if (is_offside(prev_index, index))
			break ;
	}
}

Board				create_random_board(void)
{
	Board board;

	srand (time(NULL));

	int players[2]{-1, 1};
	int	offsets[4]{RIGHT, DOWN, DIAGUPR, DIAGDWNR};
	int start_pos;

	for (int i = 1; i < 6; i++)
	{
		for (auto offset: offsets)
		{
			for (auto player : players)
			{
				start_pos = rand() % (BOARD_LENGTH*BOARD_LENGTH);
				place_pieces(board, player, start_pos, i, offset);
			}
		}
	}
	return board;
}

Board				create_random_board(int seed)
{
	Board board;

	srand (seed);

	int players[2]{-1, 1};
	int	offsets[4]{RIGHT, DOWN, DIAGUPR, DIAGDWNR};
	int start_pos;

	for (int i = 1; i < 6; i++)
	{
		for (auto offset: offsets)
		{
			for (auto player : players)
			{
				start_pos = rand() % (BOARD_LENGTH*BOARD_LENGTH);
				place_pieces(board, player, start_pos, i, offset);
			}
		}
	}
	return board;
}

Board				create_random_board(std::vector<int> &filled_positions)
{
	Board board;

	srand (time(NULL));

	int players[2]{-1, 1};
	int	offsets[4]{RIGHT, DOWN, DIAGUPR, DIAGDWNR};
	int start_pos;

	for (int i = 1; i < 6; i++)
	{
		for (auto offset: offsets)
		{
			for (auto player : players)
			{
				start_pos = rand() % (BOARD_LENGTH*BOARD_LENGTH);
				place_pieces(board, player, start_pos, i, offset, filled_positions);
			}
		}
	}
	return board;
}

Board				create_random_board(int seed, std::vector<int> &filled_positions)
{
	Board board;

	srand (seed);

	int players[2]{-1, 1};
	int	offsets[4]{RIGHT, DOWN, DIAGUPR, DIAGDWNR};
	int start_pos;

	for (int i = 1; i < 6; i++)
	{
		for (auto offset: offsets)
		{
			for (auto player : players)
			{
				start_pos = rand() % (BOARD_LENGTH*BOARD_LENGTH);
				place_pieces(board, player, start_pos, i, offset, filled_positions);
			}
		}
	}
	return board;
}

static void			erase_el(std::vector<int> &vec, int value)
{
    vec.erase(std::remove(vec.begin(), vec.end(), value), vec.end());
}

inline double		map_value(double value, double in_minrange, double in_maxrange, double out_minrange, double out_maxrange)
{
    return ((double)value-in_minrange)/(in_maxrange-in_minrange)*(out_maxrange-out_minrange)+out_minrange;
}

std::vector<int>   create_empty_array(void)
{
    std::vector<int> empty;

    for (int i = 0; i < BOARDSIZE; i++)
        empty.push_back(i);
    return empty;
}

int    				play_random_game(Board board, int player, bool verbose)
{
    int index;
	srand(time(NULL));

	auto empty = create_empty_array();
    while (true)
    {
        index = rand() % empty.size();
        board.place(index, player);
        erase_el(empty, index);
        if (board.is_game_won())
            break ;
		if (board.is_full())
		{
			player = 0;
			break ;
		}
        player = -player;
    }
    if (verbose)
        board.show_last_move();
	return player;
}

int    				play_random_game(Board board, int player, std::vector<int> empty, bool verbose)
{
    int index;

    while (true)
    {
        index = rand() % empty.size();
        board.place(index, player);
        erase_el(empty, index);
        if (board.is_game_won())
            break ;
		if (board.is_full())
		{
			player = 0;
			break ;
		}
        player = -player;
    }
    if (verbose)
        board.show_last_move();
	return player;
}

int					play_random_games(Board &board, int start_player, int amount, bool verbose)
{
	srand(time(NULL));
	auto empty = create_empty_array();
	int wins = 0;

	for (int i = 0; i < amount; i++)
		wins += play_random_game(board, start_player, empty, verbose) == start_player ? 1 : 0;
	return wins;
}	

int					play_random_games(Board &board, int start_player, int amount, bool verbose, int seed)
{
	srand(seed);
	auto empty = create_empty_array();
	int wins = 0;

	for (int i = 0; i < amount; i++)
		wins += play_random_game(board, start_player, empty, verbose) == start_player ? 1 : 0;
	return wins;
}
