#include "BoardHeuristic.hpp"
#include "misc.hpp"

BoardHeuristic::BoardHeuristic(void) { }

BoardHeuristic::~BoardHeuristic() { }

int				BoardHeuristic::go_down(const Board *board, int index, int player, int size)
{
	int length = 0;

	for (int i = 0; i < size; i++)
	{
		index += BOARD_LENGHT;
		if (index >= BOARDSIZE || board->get_player(index) != player)
			break ;
		length++;
	}
	return length;
}

int				BoardHeuristic::go_up(const Board *board, int index, int player, int size)
{
	int length = 0;

	for (int i = 0; i < size; i++)
	{
		index -= BOARD_LENGHT;
		if (index < 0 || board->get_player(index) != player)
			break ;
		length++;
	}
	return length;
}

int				BoardHeuristic::go_left(const Board *board, int index, int player, int size)
{
	int length = 0;
	int col = get_col(index);
	int offside = col < size ?  col : size;

	for (int i = 0; i < offside; i++)
	{
		index--;
		if (board->get_player(index) != player)
			break ;
		length++;
		
	}
	return length;
}

int				BoardHeuristic::go_right(const Board *board, int index, int player, int size)
{
	int length = 0;
	int offset = (BOARD_LENGHT-1) - get_col(index);
	int offside = offset < size ?  offset : size;

	for (int i = 0; i < offside; i++)
	{
		index++;
		if (board->get_player(index) != player)
			break ;
		length++;
		
	}
	return length;
}

int				BoardHeuristic::diag_upR(const Board *board, int index, int player, int size)
{
	int length = 0;
	int offset = (BOARD_LENGHT-1) - get_col(index);
	int offside = offset < size ?  offset : size;

	for (int i = 0; i < offside; i++)
	{
		index -= (BOARD_LENGHT -1);
		if (index < 0 || board->get_player(index) != player)
			break ;
		length++;
		
	}
	return length;
}

int				BoardHeuristic::diag_downL(const Board *board, int index, int player, int size)
{
	int length = 0;
	int col = get_col(index);
	int offside = col < size ?  col : size;

	for (int i = 0; i < offside; i++)
	{
		index += (BOARD_LENGHT-1);
		if (index >= BOARDSIZE || board->get_player(index) != player)
			break ;
		length++;
		
	}
	return length;
}

int				BoardHeuristic::diag_upL(const Board *board, int index, int player, int size)
{
	int length = 0;
	int col = get_col(index);
	int offside = col < size ?  col : size;

	for (int i = 0; i < offside; i++)
	{
		index -= (BOARD_LENGHT+1);
		if (index < 0 || board->get_player(index) != player)
			break ;
		
		length++;
	}
	return length;
}

int				BoardHeuristic::diag_downR(const Board *board, int index, int player, int size)
{
	int length = 0;
	int offset = (BOARD_LENGHT-1) - get_col(index);
	int offside = offset < size ?  offset : size;

	for (int i = 0; i < offside; i++)
	{
		index += (BOARD_LENGHT+1);
		if (index >= BOARDSIZE || board->get_player(index) != player)
			break ;
		
		length++;
	}
	return length;
}

int				BoardHeuristic::count_diag_down(const Board *board) const
{
	int total = 1;

	total += diag_upL(board, board->get_last_move(), board->get_last_player(), 4);
	total += diag_downR(board, board->get_last_move(), board->get_last_player(), 4);
	return total;
}

int				BoardHeuristic::count_diag_up(const Board *board) const
{
	int total = 1;

	total += diag_upR(board, board->get_last_move(), board->get_last_player(), 4);
	total += diag_downL(board, board->get_last_move(), board->get_last_player(), 4);
	return total;
}

int				BoardHeuristic::count_ver(const Board *board) const
{
	int total = 1;

	total += go_up(board, board->get_last_move(), board->get_last_player(), 4);
	total += go_down(board, board->get_last_move(), board->get_last_player(), 4);
	return total;
}

int				BoardHeuristic::count_hor(const Board *board) const
{
	int total = 1;

	total += go_left(board, board->get_last_move(), board->get_last_player(), 4);
	total += go_right(board, board->get_last_move(), board->get_last_player(), 4);
	return total;
}

bool			BoardHeuristic::check_win(const Board *board) const
{
	return (
		board->get_captures(PLAYER1) >= CAPTUREWIN || \
		board->get_captures(PLAYER2) >= CAPTUREWIN || \
		count_hor(board) >= WINCONDITION || \
		count_ver(board) >= WINCONDITION || \
		count_diag_down(board) >= WINCONDITION || \
		count_diag_up(board) >= WINCONDITION);
}

short			BoardHeuristic::get_pattern(int index, int player) const
{
	return 0;
}
