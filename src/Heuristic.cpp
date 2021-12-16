# include "Heuristic.hpp"
# include "Board.hpp"
# include "misc.hpp"

Heuristic::Heuristic(void) { }

Heuristic::~Heuristic() { }

int				Heuristic::determine_score(int count, int gaps, int open) const
{
	int score = 0;
	// placeholder
	int gap_penalty = 0;

	if (count + gaps + open)
	gap_penalty += gaps;
	if (gaps == 0)
		gap_penalty += 1;
	score += POINTS[count];
	score /= gap_penalty;
	return score;
}

bool			Heuristic::get_direction(const Board *board, int move, int direction, int player) const
{
	int pos;
	int gaps = 0;
	int count = 1;
	int open = 0;
	int shift;
	int score;
	for (int j = 0; j < 2; j++)
	{
		pos = move;
		shift = DIRECTIONS[direction + 4 * j];
		if (gaps > 1)
			return false;
		for (int i = 0; i < 4; i++)
		{
			pos += shift;
			if (is_offside(pos - shift, pos))
				break;
			if (board->get_player_id(pos) == player)
				count++;
			else if (board->get_player_id(pos) == -player)
				break;
			else if (!(is_offside(pos, pos + shift)) && board->get_player_id(pos + shift) == player)
				gaps++;
			else
			{
				open++;
				break;
			}
		}
	}
	
	score += determine_score(count, gaps, open);

	if (count == 3 && gaps < 2 && open + gaps > 2)
		return false;
	return true;
}

int				Heuristic::count_direction(const Board *board, int index, int player, int dir, int size, std::bitset<BOARDSIZE> &checked_indices) const
{
	int length = 0;
	int prev_index;

	for (int i = 0; i < size - 1; i++)
	{
		prev_index = index;
		index += dir;
		if (is_offside(prev_index, index) || board->get_player_id(index) != player)
			break ;
		checked_indices[index] = 1;
		length++;
	}
	return length;
}

int				Heuristic::count_direction(const Board *board, int index, int player, int dir, int size) const
{
	int length = 0;
	int prev_index;

	for (int i = 0; i < size - 1; i++)
	{
		prev_index = index;
		index += dir;
		if (is_offside(prev_index, index) || board->get_player_id(index) != player)
			break ;
		length++;
	}
	return length;
}

int				Heuristic::count_both_dir(const Board *board, int index, int player, int dir, std::bitset<BOARDSIZE> &checked_indices) const
{
	int total = 1;

	total += count_direction(board, index, player, -(dir), 5, checked_indices);
	total += count_direction(board, index, player, dir, 5, checked_indices);
	return total;
}

int				Heuristic::count_both_dir(const Board *board, int index, int player, int dir) const
{
	int total = 1;

	total += count_direction(board, index, player, -(dir), 5);
	total += count_direction(board, index, player, dir, 5);
	return total;
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

bool			Heuristic::continue_game(const Board *board, Player &player) const
{
	Player op_player = *player.next;
	Board tmp;

	for (int i = 0; i < BOARDSIZE; i++)
	{
		if (!board->is_empty_place(i))
			continue;
		tmp = board->get_copy();
		if ((tmp.check_captures(op_player, i) + op_player.captures) >= CAPTUREWIN
		|| !this->check_wincodition_all_dir(&tmp, player.last_move, player.id))
			return true;
	}
	return false;
}

bool			Heuristic::still_winning(const Board *board, Player &player) const
{
	if (this->check_wincodition_all_dir(board, player.winning_index, player.id))
		return true;
	player.winning_index = -1;
	return false;
}

bool			Heuristic::has_won(const Board *board, Player &player) const
{
	if (this->check_wincodition_all_dir(board, player.last_move, player.id))
	{
		player.winning_index = player.last_move;
		return !this->continue_game(board, player);
	}
	return false;
}

int				Heuristic::eight_directions_heuristic(Board *board, int index, std::bitset<BOARDSIZE> &checked_indices, int player)
{
	int points = 0;


	points += POINTS[count_both_dir(board, index, player, RIGHT, checked_indices)];
	points += POINTS[count_both_dir(board, index, player, DIAGDWNR, checked_indices)];
	points += POINTS[count_both_dir(board, index, player, DOWN, checked_indices)];
	points += POINTS[count_both_dir(board, index, player, DIAGDWNL, checked_indices)];

	return points;
}

int				Heuristic::get_heuristic_from_player(Board *board, Player &player)
{
	int points = 0;

	points += POINTS[count_both_dir(board, player.last_move, player.id, RIGHT)];
	points += POINTS[count_both_dir(board, player.last_move, player.id, DIAGDWNR)];
	points += POINTS[count_both_dir(board, player.last_move, player.id, DOWN)];
	points += POINTS[count_both_dir(board, player.last_move, player.id, DIAGDWNL)];

	return points;
}

int				Heuristic::calc_heuristic(Board *board)
{
	std::bitset<BOARDSIZE> checked_indices = 0;
	int total_score = 0;
	int	player = 0;

	for (int index = 0; index < board->filled_pos.size(); index++)
	{
		if (board->is_empty_place(index))
			continue;
		if (checked_indices[index])
			continue;
		player = board->get_player_id(index);
		if (player == 0)
			std::cout << index << std::endl;
		total_score += eight_directions_heuristic(board, index, checked_indices, player);
		checked_indices[index] = 1;
	}
	if (board->filled_pos.size() != checked_indices.count())
	{
		board->print();
		for (int i = 0; i < board->filled_pos.size(); i++)
		{
			if (board->is_empty_place(i))
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
