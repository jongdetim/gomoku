#include "misc.hpp"
#include "heuristic.hpp"

int				get_col(int index)
{
	return (index % BOARD_LENGHT);
}

int				get_row(int index)
{
	return (index / BOARD_LENGHT);
}

int				calc_index(int row, int col)
{
	return (row * BOARD_LENGHT + col);
}

int				count_bits(unsigned int number)
{
	return (int)log2(number)+1;
}

int				count_set_bits(unsigned int number)
{
	int count = 0;
	while (number)
	{
		count += number & 1;
		number >>= 1;
	}
	return count;
}

void    		print_bitboard(BITBOARD bitmap)
{
	std::cout << std::endl << "   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8" << std::endl;
	for (int row = 0; row < BOARD_LENGHT; row++)
	{
		std::cout << row%10 << ": ";
		for (int col = 0; col < BOARD_LENGHT; col++)
		{
			int index = (row * BOARD_LENGHT + col) << 1;
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

static bool		is_offside(int prev_index, int index)
{
	int row = get_row(index), col = get_col(index);
	int prev_row = get_row(prev_index), prev_col = get_col(prev_index);

	return ((abs(prev_row - row) > 1) or (abs(prev_col - col) > 1));
}

void			place_pieces(Board &board, int player, int start_pos, int amount, int offset, std::vector<int> &filled_positions)
{
	int index = start_pos;
	int prev_index;

	assert(start_pos >= 0 and start_pos < (BOARD_LENGHT*BOARD_LENGHT));
	for (int i = 1; i <= amount; i++)
	{
		if (index >= (BOARD_LENGHT*BOARD_LENGHT) or index < 0)
			break ;
		board.place(index, player);
		prev_index = index;
		index = start_pos + (i * offset);
		if (is_offside(prev_index, index))
			break ;
		filled_positions.push_back(index);
	}
}

void			place_pieces(Board &board, int player, int start_pos, int amount, int offset)
{
	int index = start_pos;
	int prev_index;

	assert(start_pos >= 0 and start_pos < (BOARD_LENGHT*BOARD_LENGHT));
	for (int i = 1; i <= amount; i++)
	{
		if (index >= (BOARD_LENGHT*BOARD_LENGHT) or index < 0)
			break ;
		board.place(index, player);
		prev_index = index;
		index = start_pos + (i * offset);
		if (is_offside(prev_index, index))
			break ;
	}
}

Board			create_random_board(void)
{
	Board board;

	srand (time(NULL));

	int players[2]{-1, 1};
	int	offsets[4]{HOR, VER, DIAG1, DIAG2};
	int start_pos;

	for (int i = 1; i < 6; i++)
	{
		for (auto offset: offsets)
		{
			for (auto player : players)
			{
				start_pos = rand() % (BOARD_LENGHT*BOARD_LENGHT);
				place_pieces(board, player, start_pos, i, offset);
			}
		}
	}
	return board;
}

Board			create_random_board(int seed)
{
	Board board;

	srand (seed);

	int players[2]{-1, 1};
	int	offsets[4]{HOR, VER, DIAG1, DIAG2};
	int start_pos;

	for (int i = 1; i < 6; i++)
	{
		for (auto offset: offsets)
		{
			for (auto player : players)
			{
				start_pos = rand() % (BOARD_LENGHT*BOARD_LENGHT);
				place_pieces(board, player, start_pos, i, offset);
			}
		}
	}
	return board;
}

static void		erase_el(std::vector<int> &vec, int value)
{
    vec.erase(std::remove(vec.begin(), vec.end(), value), vec.end());
}

void    		play_random_game(Board &board, int player, std::vector<int> empty, bool verbose)
{
	srand (time(NULL));
    int index;

    while (true)
    {
        index = rand() % empty.size();
        board.place(index, player);
        erase_el(empty, index);
        if (check_win(board) or board.is_full())
            break ;
        player = -player;
    }
    if (verbose)
        board.show_last_move();
}

std::vector<int>   create_empty_array(void)
{
    std::vector<int> empty;

    for (int i = 0; i < BOARDSIZE; i++)
        empty.push_back(i);
    return empty;
}

void		play_random_games(Board &board, int start_player, int amount, bool verbose)
{
	auto empty = create_empty_array();

	for (int i = 0; i < amount; i++)
		play_random_game(board, start_player, empty, verbose);
}	