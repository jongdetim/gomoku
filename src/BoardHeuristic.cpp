#include "BoardHeuristic.hpp"
#include "misc.hpp"
#include "Board.hpp"

BoardHeuristic::BoardHeuristic(void) { }

BoardHeuristic::~BoardHeuristic() { }


static bool		is_offside(int index, int prev_index)
{
	int row = get_row(index), col = get_col(index);
	int prev_row = get_row(prev_index), prev_col = get_col(prev_index);

	return ((abs(prev_row - row) > 1) or (abs(prev_col - col) > 1));
}

int				BoardHeuristic::count_direction(const Board *board, int index, int player, int size, int dir) const
{
	int length = 0;
	int prev_index = index;

	for (int i = 0; i < size; i++)
	{
		if (is_offside(index, prev_index) || board->get_player(index) != player)
			break ;
		length++;
		prev_index = index;
		index += dir;
	}
	return length;
}

int				BoardHeuristic::count_diag_down(const Board *board) const
{
	int total = 0;

	total += count_direction(board, board->get_last_move(), board->get_last_player(), 5, DIAGUPL);
	total += count_direction(board, board->get_last_move(), board->get_last_player(), 5, DIAGDWNR);
	return total - 1;
}

int				BoardHeuristic::count_diag_up(const Board *board) const
{
	int total = 0;

	total += count_direction(board, board->get_last_move(), board->get_last_player(), 5, DIAGUPR);
	total += count_direction(board, board->get_last_move(), board->get_last_player(), 5, DIAGDWNL);
	return total - 1;
}

int				BoardHeuristic::count_ver(const Board *board) const
{
	int total = 0;

	total += count_direction(board, board->get_last_move(), board->get_last_player(), 5, UP);
	total += count_direction(board, board->get_last_move(), board->get_last_player(), 5, DOWN);
	return total - 1;
}

int				BoardHeuristic::count_hor(const Board *board) const
{
	int total = 0;

	total += count_direction(board, board->get_last_move(), board->get_last_player(), 5, LEFT);
	total += count_direction(board, board->get_last_move(), board->get_last_player(), 5, RIGHT);
	return total - 1;
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
