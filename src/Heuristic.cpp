# include "heuristic.hpp"
# include "Board.hpp"
# include "misc.hpp"

int				heuristic::determine_score(int count, int gaps, int open)
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

bool			heuristic::get_direction(const Board &board, int move, int direction, int player_id)
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
			if (misc::is_offside(pos - shift, pos))
				break;
			if (board.get_player_id(pos) == player_id)
				count++;
			else if (board.get_player_id(pos) == -player_id)
				break;
			else if (!(misc::is_offside(pos, pos + shift)) && board.get_player_id(pos + shift) == player_id)
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

int				heuristic::count_direction(const Board &board, int index, int player_id, int dir, int size, std::bitset<BOARDSIZE> &checked_indices)
{
	int length = 0;
	int prev_index;

	for (int i = 0; i < size - 1; i++)
	{
		prev_index = index;
		index += dir;
		if (misc::is_offside(prev_index, index) || board.get_player_id(index) != player_id)
			break ;
		checked_indices[index] = 1;
		length++;
	}
	return length;
}

int				heuristic::count_direction(const Board &board, int index, int player_id, int dir, int size)
{
	int length = 0;
	int prev_index;

	for (int i = 0; i < size - 1; i++)
	{
		prev_index = index;
		index += dir;
		if (misc::is_offside(prev_index, index) || board.get_player_id(index) != player_id)
			break ;
		length++;
	}
	return length;
}

int				heuristic::count_both_dir(const Board &board, int index, int player_id, int dir, std::bitset<BOARDSIZE> &checked_indices)
{
	int total = 1;

	total += count_direction(board, index, player_id, -(dir), 5, checked_indices);
	total += count_direction(board, index, player_id, dir, 5, checked_indices);
	return total;
}

int				heuristic::count_both_dir(const Board &board, int index, int player_id, int dir)
{
	int total = 1;

	total += count_direction(board, index, player_id, -(dir), 5);
	total += count_direction(board, index, player_id, dir, 5);
	return total;
}

int				heuristic::eight_directions_heuristic(Board &board, int index, std::bitset<BOARDSIZE> &checked_indices, int player_id)
{
	int points = 0;

	points += POINTS[count_both_dir(board, index, player_id, RIGHT, checked_indices)];
	points += POINTS[count_both_dir(board, index, player_id, DIAGDWNR, checked_indices)];
	points += POINTS[count_both_dir(board, index, player_id, DOWN, checked_indices)];
	points += POINTS[count_both_dir(board, index, player_id, DIAGDWNL, checked_indices)];

	return points;
}

int				heuristic::get_heuristic_from_player(Board &board, const Player &player)
{
	int points = 0;

	points += POINTS[count_both_dir(board, player.last_move, player.id, RIGHT)];
	points += POINTS[count_both_dir(board, player.last_move, player.id, DIAGDWNR)];
	points += POINTS[count_both_dir(board, player.last_move, player.id, DOWN)];
	points += POINTS[count_both_dir(board, player.last_move, player.id, DIAGDWNL)];

	return points;
}

int				heuristic::calc_heuristic(Board &board)
{
	std::bitset<BOARDSIZE> checked_indices = 0;
	int total_score = 0;
	int	player_id = 0;

	for (int index = 0; index < board.filled_pos.size(); index++)
	{
		if (board.is_empty_place(index))
			continue;
		if (checked_indices[index])
			continue;
		player_id = board.get_player_id(index);
		if (player_id == 0)
			std::cout << index << std::endl;
		total_score += eight_directions_heuristic(board, index, checked_indices, player_id);
		checked_indices[index] = 1;
	}
	if (board.filled_pos.size() != checked_indices.count())
	{
		board.print();
		for (int i = 0; i < board.filled_pos.size(); i++)
		{
			if (board.is_empty_place(i))
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
