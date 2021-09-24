#include "misc.hpp"

int		calc_index(int row, int col)
{
	return (row * BOARDSIZE + col);
}

int		count_bits(unsigned int number)
{
	return (int)log2(number)+1;
}

int		count_set_bits(unsigned int number)
{
	int count = 0;
	while (number)
	{
		count += number & 1;
		number >>= 1;
	}
	return count;
}

void    print_bitmap(std::bitset<MASKSIZE> bitmap)
{
	std::cout << std::endl << "   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8" << std::endl;
	for (int row = 0; row < BOARDSIZE; row++)
	{
		std::cout << row%10 << ": ";
		for (int col = 0; col < BOARDSIZE; col++)
		{
			int index = (row * BOARDSIZE + col) << 1;
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

void	place_pieces(Board &board, int player, int start_pos, int amount, int offset, std::vector<int> &filled_positions)
{
	int index;

	assert(start_pos >= 0 and start_pos < (BOARDSIZE*BOARDSIZE));
	for (int i = 0; i < amount; i++)
	{
		index = start_pos + (i * offset);
		if (index >= (BOARDSIZE*BOARDSIZE) or index < 0)
			break ;
		filled_positions.push_back(index);
		board.place(index, player);
	}
}

void	place_pieces(std::bitset<MASKSIZE> &board, int player, int start_pos, int amount, int offset)
{
	int index;

	assert(start_pos >= 0 and start_pos < (BOARDSIZE*BOARDSIZE));
	for (int i = 0; i < amount; i++)
	{
		index = start_pos + (i * offset);
		if (index >= (BOARDSIZE*BOARDSIZE) or index < 0)
			break ;
		index <<= 1;
		if (board[index] == false and board[index+1] == false)
		{
			index = player == PLAYER1 ? index : index + 1;
			board[index] = true;
		}	
	}
}

void	place_pieces(Board &board, int player, int start_pos, int amount, int offset)
{
	int index;

	assert(start_pos >= 0 and start_pos < (BOARDSIZE*BOARDSIZE));
	for (int i = 0; i < amount; i++)
	{
		index = start_pos + (i * offset);
		if (index >= (BOARDSIZE*BOARDSIZE) or index < 0)
			break ;
		board.place(index, player);
	}
}

Board	create_random_board(void)
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
				start_pos = rand() % (BOARDSIZE*BOARDSIZE);
				place_pieces(board, player, start_pos, i, offset);
			}
		}
	}
	return board;
}

Board	create_random_board(int seed)
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
				start_pos = rand() % (BOARDSIZE*BOARDSIZE);
				place_pieces(board, player, start_pos, i, offset);
			}
		}
	}
	return board;
}
