#include "BoardHeuristic.hpp"
#include "misc.hpp"
#include "Board.hpp"

BoardHeuristic::BoardHeuristic(void) { }

BoardHeuristic::~BoardHeuristic() { }

int				BoardHeuristic::count_direction(const Board *board, int index, int player, int dir, int size) const
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

int				BoardHeuristic::count_both_dir(const Board *board, int index, int player, int dir) const
{
	int total = 0;

	total += count_direction(board, index, player, -(dir), 5);
	total += count_direction(board, index, player, dir, 5);
	return total - 1;
}

bool			BoardHeuristic::check_wincodition_all_dir(const Board *board, int index, int player) const
{
	int directions[4] = {DOWN, RIGHT, DIAGDWNL, DIAGDWNR};

	for (int i = 0; i < 4; i++)
	{
		if (this->count_both_dir(board, index, player, directions[i]) >= WINCONDITION)
			return true;
	}
	return false;
}

bool			BoardHeuristic::continue_game(const Board *board, int index, int player) const
{
	int op_player = -player;
	int captures = board->get_player_captures(op_player);
	Board tmp;


	for (int i = 0; i < BOARDSIZE; i++)
	{
		if (board->filled_pos[i])
			continue;
		tmp = *board;
		if ((tmp.check_captures(op_player, i) + captures) >= CAPTUREWIN
		|| !this->check_wincodition_all_dir(&tmp, index, player))
			return true;
	}
	return false;
}

bool			BoardHeuristic::has_won(const Board *board, int index, int player) const
{
	if (board->get_player_captures(player) >= CAPTUREWIN)
		return true;
	if (this->check_wincodition_all_dir(board, index, player))
		return !this->continue_game(board, index, player);
	return false;
}
