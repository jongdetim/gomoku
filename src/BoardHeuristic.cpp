#include "BoardHeuristic.hpp"
#include "misc.hpp"

BoardHeuristic::BoardHeuristic(Board *board) : board(board) { }

BoardHeuristic::~BoardHeuristic() { }

int				BoardHeuristic::go_down(Board *board, int index, int player, int size)
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

int				BoardHeuristic::go_up(Board *board, int index, int player, int size)
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

int				BoardHeuristic::go_left(Board *board, int index, int player, int size)
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

int				BoardHeuristic::go_right(Board *board, int index, int player, int size)
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

int				BoardHeuristic::diag_upR(Board *board, int index, int player, int size)
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

int				BoardHeuristic::diag_downL(Board *board, int index, int player, int size)
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

int				BoardHeuristic::diag_upL(Board *board, int index, int player, int size)
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

int				BoardHeuristic::diag_downR(Board *board, int index, int player, int size)
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

int				BoardHeuristic::count_diag_down(void) const
{
	int total = 1;

	total += diag_upL(this->board, this->board->get_last_move(), this->board->get_last_player(), 4);
	total += diag_downR(this->board, this->board->get_last_move(), this->board->get_last_player(), 4);
	return total;
}

int				BoardHeuristic::count_diag_up(void) const
{
	int total = 1;

	total += diag_upR(this->board, this->board->get_last_move(), this->board->get_last_player(), 4);
	total += diag_downL(this->board, this->board->get_last_move(), this->board->get_last_player(), 4);
	return total;
}

int				BoardHeuristic::count_ver(void) const
{
	int total = 1;

	total += go_up(this->board, this->board->get_last_move(), this->board->get_last_player(), 4);
	total += go_down(this->board, this->board->get_last_move(), this->board->get_last_player(), 4);
	return total;
}

int				BoardHeuristic::count_hor(void) const
{
	int total = 1;

	total += go_left(this->board, this->board->get_last_move(), this->board->get_last_player(), 4);
	total += go_right(this->board, this->board->get_last_move(), this->board->get_last_player(), 4);
	return total;
}

bool			BoardHeuristic::check_win(void) const
{
	return (count_hor() == 5 \
	|| count_ver() == 5 \
	|| count_diag_down() == 5 \
	|| count_diag_up() == 5);
}
