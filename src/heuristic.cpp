#include "heuristic.hpp"
#include "misc.hpp"

// #define PRINT(x) std::cout << x << std::endl
// #define PRINTENDL std::cout << std::endl


int						get_hor_pattern(Board &board, int start_index, int player) // <---- TESTING
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
////////////////////////////////////////////////////////////
int						g_points[6]{0,0,ROW2,ROW3,ROW4,ROW5};

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
	//std::cout << "VER:       " << total << " points: " << g_points[total] << std::endl;
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
	//std::cout << "HOR:       " << total  << " points: " << g_points[total] << std::endl;
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

int						count_diag_up(Board &board, int index, int player)
{
	int total = 1;

	total += diag_upR(board, index, player);
	total += diag_downL(board, index, player);
	//std::cout << "DIAG_UP:   " << total  << " points: " << g_points[total] << std::endl;
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

int						count_diag_down(Board &board, int index, int player)
{
	int total = 1;

	total += diag_upL(board, index, player);
	total += diag_downR(board, index, player);
	//std::cout << "DIAG_DOWN: " << total  << " points: " << g_points[total] << std::endl;
	return total;
}

int						get_heuristic_last_move(Board &board)
{
	int points = 0;

	points += g_points[count_hor(board, board.last_move, board.get_last_player())];
	points += g_points[count_ver(board, board.last_move, board.get_last_player())];
	points += g_points[count_diag_down(board, board.last_move, board.get_last_player())];
	points += g_points[count_diag_up(board, board.last_move, board.get_last_player())];

	return points;
}

bool					check_win(Board &board)
{
	return (count_hor(board, board.last_move, board.get_last_player()) == 5 \
	or count_ver(board, board.last_move, board.get_last_player()) == 5 \
	or count_diag_down(board, board.last_move, board.get_last_player()) == 5 \
	or count_diag_up(board, board.last_move, board.get_last_player()) == 5);
}

int						calc_heuristic(Board &state)
{
	int points = 0;

	return points;
}
