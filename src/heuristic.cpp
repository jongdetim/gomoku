# include "Heuristic.hpp"
# include "Board.hpp"
# include "misc.hpp"

Heuristic::Heuristic(void) { }

Heuristic::~Heuristic() { }

// Pattern search_subpattern(t_pattern &pat, t_pattern &sub)
// {
//     uint8_t times = pat.length - sub.length;

//     for (uint8_t i = 0; i < times + 1; i++)
//     {
//         if(uint8_t(pat.pattern << i) >> times == sub.pattern)
//         {
//             std::cout << std::bitset<8>(uint8_t(pat.pattern << i) >> times) << std::endl;
//             // pattern found, do the thing
//         }
//     }
//     return none;
// }


// int				Heuristic::determine_score(int count, int gaps, int open) const
// {
// 	int score = 0;
// 	// placeholder
// 	int gap_penalty = 0;

// 	if (count + gaps + open)
// 	gap_penalty += gaps;
// 	if (gaps == 0)
// 		gap_penalty += 1;
// 	score += POINTS[count];
// 	score /= gap_penalty;
// 	return score;
// }

// bool			Heuristic::get_direction(const Board *board, int move, int direction, int player) const
// {
// 	int pos;
// 	int gaps = 0;
// 	int count = 1;
// 	int open = 0;
// 	int shift;
// 	int score;
// 	for (int j = 0; j < 2; j++)
// 	{
// 		pos = move;
// 		shift = DIRECTIONS[direction + 4 * j];
// 		if (gaps > 1)
// 			return false;
// 		for (int i = 0; i < 4; i++)
// 		{
// 			pos += shift;
// 			if (is_offside(pos - shift, pos))
// 				break;
// 			if (board->get_player(pos) == player)
// 				count++;
// 			else if (board->get_player(pos) == -player)
// 				break;
// 			else if (!(is_offside(pos, pos + shift)) && board->get_player(pos + shift) == player)
// 				gaps++;
// 			else
// 			{
// 				open++;
// 				break;
// 			}
// 		}
// 	}
	
// 	score += determine_score(count, gaps, open);

// 	if (count == 3 && gaps < 2 && open + gaps > 2)
// 		return false;
// 	return true;
// }

int				Heuristic::count_direction(const Board *board, int index, int player, int dir, int size, std::bitset<BOARDSIZE> &checked_indices)
{
	int length = 0;
	int prev_index;

	for (int i = 0; i < 4; i++)
	{
		prev_index = index;
		index += dir;
		if (is_offside(index, prev_index) || board->get_player(index) != player)
			break ;
		checked_indices[index] = 1;
		length++;
	}
	return length;
}

int				Heuristic::count_direction(const Board *board, int index, int player, int dir, int size)
{
	int length = 0;
	int prev_index;

	for (int i = 0; i < size; i++)
	{
		prev_index = index;
		index += dir;
		if (is_offside(index, prev_index) || board->get_player(index) != player)
			break ;
		length++;
	}
	return length;
}

int				Heuristic::count_both_dir(const Board *board, int index, int player, int dir, std::bitset<BOARDSIZE> &checked_indices)
{
	int total = 1;

	total += count_direction(board, index, player, -(dir), 5, checked_indices);
	total += count_direction(board, index, player, dir, 5, checked_indices);
	return total;
}

int				Heuristic::count_both_dir(const Board *board, int index, int player, int dir)
{
	int total = 1;

	total += count_direction(board, index, player, -(dir), 5);
	total += count_direction(board, index, player, dir, 5);
	return total;
}

bool			Heuristic::check_wincodition_all_dir(const Board *board, int index, int player)
{
	int directions[4] = {DOWN, RIGHT, DIAGDWNL, DIAGDWNR};

	for (int i = 0; i < 4; i++)
	{
		if (count_both_dir(board, index, player, directions[i]) >= WINCONDITION)
			return true;
	}
	return false;
}

bool			Heuristic::continue_game(const Board *board, int index, int player)
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
		|| !check_wincodition_all_dir(&tmp, index, player))
			return true;
	}
	return false;
}

bool			Heuristic::has_won(const Board *board, int index, int player)
{
	if (board->get_player_captures(player) >= CAPTUREWIN)
		return true;
	if (check_wincodition_all_dir(board, index, player))
		return !continue_game(board, index, player);
	return false;
}

int				Heuristic::eight_directions_heuristic(Board *board, int index, std::bitset<BOARDSIZE> &checked_indices, int player)
{
	int points = 0;


	points += POINTS[count_both_dir(board, board->get_last_move(), board->get_last_player(), RIGHT, checked_indices)];
	points += POINTS[count_both_dir(board, board->get_last_move(), board->get_last_player(), DIAGDWNR, checked_indices)];
	points += POINTS[count_both_dir(board, board->get_last_move(), board->get_last_player(), DOWN, checked_indices)];
	points += POINTS[count_both_dir(board, board->get_last_move(), board->get_last_player(), DIAGDWNL, checked_indices)];

	return player * points;
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
		player = board->get_player(index);
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
