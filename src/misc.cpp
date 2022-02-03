#include "misc.hpp"
#include "Board.hpp"

void						Timer::start()
{
	m_StartTime = std::chrono::steady_clock::now();
	m_bRunning = true;
}

void						Timer::stop()
{
	m_EndTime = std::chrono::steady_clock::now();
	m_bRunning = false;
}

int							Timer::elapsedMilliseconds()
{
	std::chrono::time_point<std::chrono::steady_clock> endTime;
	
	if(m_bRunning)
		endTime = std::chrono::steady_clock::now();
	else
		endTime = m_EndTime;
	return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
}

double						Timer::elapsedSeconds()
{
	return elapsedMilliseconds() / 1000.0;
}

int							misc::random_int(void)
{
   static bool first = true;
   if (first) 
   {
		srand( time(NULL) );
		first = false;
   }
   return rand();
}

int							misc::random(int min, int max, int seed)
{
   static bool first = true;
   if (first) 
   {  
		srand( seed );
		first = false;
   }
   return min + rand() % (( max + 1 ) - min);
}

int							misc::random(int min, int max)
{
   static bool first = true;
   if (first) 
   {  
		srand( time(NULL) );
		first = false;
   }
   return min + rand() % (( max + 1 ) - min);
}

int							misc::get_col(int index) { return (index % BOARD_LENGTH); }

int							misc::get_row(int index) { return (index / BOARD_LENGTH); }

int							misc::calc_index(int row, int col)
{
	if (row < 0 || row >= 19 || col < 0 || col >= 19)
		return -1;
	return (row * BOARD_LENGTH + col);
}

void    					misc::print_and_quit(const char *msg)
{
    PRINT(msg);
    exit(1);
}

void						misc::print_stats()
{
	PRINT("Total nodes: " << TOTAL_NODES);
	PRINT("Total leaves: " << TOTAL_LEAVES << "\n");
	PRINT("Total found in table: " << FOUND_IN_TABLE);
	PRINT("Total times branches pruned: " << TOTAL_BRANCHES_PRUNED << "\n");
}

bool						misc::is_offside(int prev_index, int index)
{
	int row = get_row(index), col = get_col(index);
	int prev_row = get_row(prev_index), prev_col = get_col(prev_index);

	return ((abs(prev_row - row) > 1) || (abs(prev_col - col) > 1) || index < 0 || index >= BOARDSIZE);
}

void						misc::create_star(Board &board, int index, int size, int player_index)
{
	board.set_current_player(player_index);
    for (auto dir : DIRECTIONS)
        place_pieces(board, index, size, dir);
}

void						misc::place_pieces(Board &board, int start_pos, int amount, int offset)
{
	int index = start_pos;
	int prev_index;

	assert(start_pos >= 0 && start_pos < (BOARDSIZE));
	for (int i = 0; i < amount; i++)
	{
		prev_index = index;
		index = start_pos + (i * offset);
		if (is_offside(prev_index, index))
			break ;
		board.place(index);
	}
}

Board						misc::create_random_board(void)
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

Board						misc::create_random_board(int seed)
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
	return board;
}

std::vector<std::string>	misc::tokenize(std::string &str, char delim)
{
	size_t start;
	size_t end = 0;
    std::vector<std::string> tokens;

	while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = str.find(delim, start);
		tokens.push_back(str.substr(start, end - start));
	}
	return tokens;
}
