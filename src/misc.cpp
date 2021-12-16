#include "misc.hpp"
#include "Board.hpp"

int					random_int(void)
{
   static bool first = true;
   if (first) 
   {  
		srand( time(NULL) );
		first = false;
   }
   return rand();
}

int					random(int min, int max)
{
   static bool first = true;
   if (first) 
   {  
		srand( time(NULL) );
		first = false;
   }
   return min + rand() % (( max + 1 ) - min);
}

int					get_col(int index) { return (index % BOARD_LENGTH); }

int					get_row(int index) { return (index / BOARD_LENGTH); }

int					calc_index(int row, int col)
{
	if (row < 0 || row >= 19 || col < 0 || col >= 19)
		return -1;
	return (row * BOARD_LENGTH + col);
}

bool				is_offside(int prev_index, int index)
{
	int row = get_row(index), col = get_col(index);
	int prev_row = get_row(prev_index), prev_col = get_col(prev_index);

	return ((abs(prev_row - row) > 1) || (abs(prev_col - col) > 1) || index < 0 || index >= BOARDSIZE);
}

void				create_star(Board &board, int index, int size, int player)
{
	board.switch_to_player(player);
    for (auto dir : DIRECTIONS)
        place_pieces(board, index, size, dir);
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

void				place_pieces(Board &board, int start_pos, int amount, int offset)
{
	int index = start_pos;
	int prev_index;

	assert(start_pos >= 0 and start_pos < (BOARDSIZE));
	for (int i = 0; i < amount; i++)
	{
		prev_index = index;
		index = start_pos + (i * offset);
		if (is_offside(prev_index, index))
			break ;
		board.place(index);
	}
}

Board				create_random_board(void)
{
	Board board;

	srand (time(NULL));

	int	offsets[4]{RIGHT, DOWN, DIAGUPR, DIAGDWNR};
	int start_pos;

	for (int amount = 1; amount < 6; amount++)
	{
		for (auto offset: offsets)
		{
			for (int i = 0; i < 2; i++)
			{
				start_pos = rand() % (BOARD_LENGTH*BOARD_LENGTH);
				place_pieces(board, start_pos, amount, offset);
				board.next_player();
			}
		}
	}
	return board;
}

Board				create_random_board(int seed)
{
	Board board;

	srand (seed);

	int	offsets[4]{RIGHT, DOWN, DIAGUPR, DIAGDWNR};
	int start_pos;

	for (int amount = 1; amount < 6; amount++)
	{
		for (auto offset: offsets)
		{
			for (int i = 0; i < 2; i++)
			{
				start_pos = rand() % (BOARD_LENGTH*BOARD_LENGTH);
				place_pieces(board, start_pos, amount, offset);
				board.next_player();
			}
		}
	}
	return board.get_copy();
}
