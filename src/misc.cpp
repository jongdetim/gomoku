#include "misc.hpp"
#include "board.hpp"

int					get_col(int index) { return (index % BOARD_LENGTH); }

int					get_row(int index) { return (index / BOARD_LENGTH); }

int					calc_index(int row, int col) { return (row * BOARD_LENGTH + col); }

void    print_and_quit(const char *msg)
{
    PRINT(msg);
    exit(1);
}

void	print_stats()
{
	PRINT("Total nodes: " << TOTAL_NODES);
	PRINT("Total leaves: " << TOTAL_LEAVES << "\n");
	PRINT("Total found in table: " << FOUND_IN_TABLE);
	PRINT("Total times branches pruned: " << TOTAL_BRANCHES_PRUNED << "\n");
}

void Timer::start()
{
	m_StartTime = std::chrono::steady_clock::now();
	m_bRunning = true;
}

void Timer::stop()
{
	m_EndTime = std::chrono::steady_clock::now();
	m_bRunning = false;
}

int Timer::elapsedMilliseconds()
{
	std::chrono::time_point<std::chrono::steady_clock> endTime;
	
	if(m_bRunning)
		endTime = std::chrono::steady_clock::now();
	else
		endTime = m_EndTime;
	return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
}

double Timer::elapsedSeconds()
{
	return elapsedMilliseconds() / 1000.0;
}

// std::chrono::milliseconds get_current_time()
// {
//     using namespace std::chrono;

// 	return duration_cast<milliseconds>(steady_clock::now().time_since_epoch());
// }

bool				is_offside(int prev_index, int index)
{
	int row = get_row(index), col = get_col(index);
	int prev_row = get_row(prev_index), prev_col = get_col(prev_index);

	return ((abs(prev_row - row) > 1) || (abs(prev_col - col) > 1) || index < 0 || index >= BOARDSIZE);
}

void				create_star(Board &board, int index, int size, int player)
{
    for (auto dir : DIRECTIONS)
        place_pieces(board, player, index, size, dir);
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

void				place_pieces(Board &board, int player, int start_pos, int amount, int offset)
{
	int index = start_pos;
	int prev_index;

	assert(start_pos >= 0 && start_pos < (BOARDSIZE));
	for (int i = 0; i < amount; i++)
	{
		prev_index = index;
		index = start_pos + (i * offset);
		if (is_offside(prev_index, index))
			break;
		board.place(index, player);
	}
}

Board				create_random_board(void)
{
	Board board;

	srand (time(NULL));

	int players[2]{PLAYER2, PLAYER1};
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
