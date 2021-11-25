# include "Heuristic.hpp"
# include "Board.hpp"
# include "misc.hpp"

Heuristic::Heuristic(void) { }

Heuristic::~Heuristic() { }

int				Heuristic::count_direction(const Board *board, int index, int player, int dir, int size) const
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

int				Heuristic::count_both_dir(const Board *board, int index, int player, int dir) const
{
	int total = 0;

	total += count_direction(board, index, player, -(dir), 5);
	total += count_direction(board, index, player, dir, 5);
	return total - 1;
}

bool			Heuristic::check_wincodition_all_dir(const Board *board, int index, int player) const
{
	int directions[4] = {DOWN, RIGHT, DIAGDWNL, DIAGDWNR};

	for (int i = 0; i < 4; i++)
	{
		if (this->count_both_dir(board, index, player, directions[i]) >= WINCONDITION)
			return true;
	}
	return false;
}

bool			Heuristic::continue_game(const Board *board, int index, int player) const
{
	int op_player = -player;
	int captures = board->get_player_captures(op_player);
	Board tmp;

	for (int i = 0; i < BOARDSIZE; i++)
	{
		if (!board->is_empty_place(i))
			continue;
		tmp = *board;
		if ((tmp.check_captures(op_player, i) + captures) >= CAPTUREWIN
		|| !this->check_wincodition_all_dir(&tmp, index, player))
			return true;
	}
	return false;
}

bool			Heuristic::has_won(const Board *board, int index, int player) const
{
	if (board->get_player_captures(player) >= CAPTUREWIN)
		return true;
	if (this->check_wincodition_all_dir(board, index, player))
		return !this->continue_game(board, index, player);
	return false;
}

int				Heuristic::eight_directions_heuristic(int index, std::bitset<BOARDSIZE> &checked_indices, int player, Board &node)
{
	int points = 0;

	// points += POINTS[count_hor(node, index, player, checked_indices)];
	// points += POINTS[count_ver(node, index, player, checked_indices)];
	// points += POINTS[count_diag_down(node, index, player, checked_indices)];
	// points += POINTS[count_diag_up(node, index, player, checked_indices)];

	return points;
}

int				Heuristic::get_heuristic_last_move(Board *board)
{
	int points = 0;

	points += POINTS[count_both_dir(board, board->get_last_move(), board->get_last_player(), RIGHT)];
	points += POINTS[count_both_dir(board, board->get_last_move(), board->get_last_player(), DIAGDWNR)];
	points += POINTS[count_both_dir(board, board->get_last_move(), board->get_last_player(), DOWN)];
	points += POINTS[count_both_dir(board, board->get_last_move(), board->get_last_player(), DIAGDWNL)];

	return points;
}

int				Heuristic::calc_heuristic(Board &node)
{
	std::bitset<BOARDSIZE> checked_indices = 0;
	int total_score = 0;
	int	player = 0;

	for (int index = 0; index < node.filled_pos.size(); index++)
	{
		if (node.is_empty_place(index))
			continue;
		if (checked_indices[index])
			continue;
		player = node.get_player(index);
		if (player == 0)
			std::cout << index << std::endl;
		total_score += eight_directions_heuristic(index, checked_indices, player, node);
		checked_indices[index] = 1;
	}
	if (node.filled_pos.size() != checked_indices.count())
	{
		node.print();
		for (int i = 0; i < node.filled_pos.size(); i++)
		{
			if (node.is_empty_place(i))
				continue;
			std::cout << i << " ";
		}
		std::cout << std::endl << "^ filled positions. check_indices v" << std::endl;
		for (int j = 0; j < BOARDSIZE; j++)
		{
			if (checked_indices[j])
				std::cout << j << " ";
		}
		std::cout << std::endl;
	}
	return total_score;
}
