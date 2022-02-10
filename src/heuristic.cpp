# include "gomoku.hpp"
# include "heuristic.hpp"
# include "Board.hpp"
# include "misc.hpp"

// this can fail in cases where length > 8, such as . o o o o . o o . where the leftmost empty space is cut off and it's seen as closed4 instead of open4. Can be solved with 16-bit cutouts.
void               heuristic::cutout_pattern(const Board &board, int move, int direction, int player, t_pattern &pat)
{
    int shift = DIRECTIONS[direction + 4];
    int pos = move - (pat.left_right[0] * shift);
    for (int i = 0; i < pat.length; i++)
    {
        pat.pattern += board.get_player(pos) == player;
        if (i != pat.length - 1)
            pat.pattern <<= 1;
        pos += shift;
    }
    // PRINT(std::bitset<8>(pat.pattern));
}

t_pattern			heuristic::get_pattern_data(Board &board, int move, int direction, int player, std::bitset<BOARDSIZE> *checked_indices)
{
	t_pattern pattern;
	int pos;
	pattern.space = 1;
	pattern.count = 1;
	int shift;
    checked_indices[direction][move] = 1;
    int enemyplayer = 1 - player;

	for (int j = 0; j < 2; j++)
	{
		pos = move;
		shift = DIRECTIONS[direction + 4 * j];
		int i = 0;
		int open = 0;
		while (open < 3)
		{
			pos += shift;
            if (open == 0)
			{
                if (misc::is_offside(pos - shift, pos) || board.get_player(pos) == enemyplayer)
                {
                    pattern.left_right[j] = i;
                    break;
                }
                if (board.get_player(pos) == player)
                {
                    pattern.count++;
                    checked_indices[direction][pos] = 1;
                }
                else if (board.get_player(pos - shift) == player) //leeg vakje maar vorige niet
                    pattern.left_right[j] = i + 1;
                else
                    open = 2;
            }
			else if (misc::is_offside(pos - shift, pos) || board.get_player(pos) == enemyplayer)
                break;
            else
				open++; 
			pattern.space++;
			i++;
		}
	}
	pattern.length = pattern.left_right[0] + pattern.left_right[1] + 1;
	return pattern;
}

int heuristic::score_remaining_patterns(Board &board, int player)
{
    int score = 0;

    for (int i = 1; i <= 5; i++) //closed2, open2, closed3, open3, closed4
        score += board.players[player].patterns[i] * SCORES[i];
    if (board.players[player].captures)
        score += pow(10, board.players[player].captures);
    return score;
}

int heuristic::evaluate_patterns(Board &board, int player)
{
    // for (uint8_t i = 0; i < 8; i++) // skip first index which is none
    // {
    //     if (i > 0 && board.players[player].patterns[i] > 0)
    //     {
    //         PRINT("player: " << player);
    //         PRINT(PatternNames[i]);
    //         PRINT((int)board.players[player].patterns[i]);
    //     }
    // }
    // board.show_last_move();

    int enemyplayer = 1 - player;
    int score = 0;
    // if (board.is_game_won()) //has to check if we have 5 pairs captured OR: 5+ in a row, enemy can't break it and can't capture 5 pairs next turn
    if (board.players[enemyplayer].captures >= 5 ||
    board.players[enemyplayer].patterns[five] >= 1)
        return -std::numeric_limits<int>::max();
    else if (board.players[player].captures >= 5 ||
    board.players[player].patterns[five] >= 1)    
        return std::numeric_limits<int>::max();
    else if (board.players[enemyplayer].patterns[closed4] >= 1 ||
    board.players[enemyplayer].patterns[open4] >= 1)
        return -std::numeric_limits<int>::max();
    else if (board.players[player].patterns[open4] >= 1 ||
    board.players[player].patterns[closed4] >= 2)
        return 1000000000;
    else if ((board.players[enemyplayer].patterns[open3] >= 1 &&
    board.players[player].patterns[closed4] == 0) ||
    board.players[enemyplayer].patterns[open3] >= 2)
        return -1000000000;
    else if (board.players[player].patterns[open3] >= 2 ||
    (board.players[player].patterns[closed4] >= 1 &&
    board.players[player].patterns[open3] >= 1))
        return 1000000;
    else
    {
        score += heuristic::score_remaining_patterns(board, player);
        score -= 1.5 * heuristic::score_remaining_patterns(board, enemyplayer);
        return score;
    }
    // we need a good way to check opponent capture options eg. can they break our 5 in a row?
    // one approach is to keep track of half-open two's (XOO.), but this won't tell us if our FIVE can be broken, of if multiple captures with 1 move are possible
    
}

// unfinished, needs to assign a score to combinations and patterns
// void evaluate_patterns_index(Board &board, int move)
// {
//     int player = board.get_player(move);
//     for (uint8_t i = 0; i < 8; i++) // skip first index which is none
//     {
//         if (i > 0 && board.players[index].patterns[i] > 0)
//         {
//             PRINT(PatternNames[i]);
//             PRINT((int)board.players[index].patterns[i]);
//         }
//     }
// }

Pattern heuristic::find_subpattern(t_pattern &pat, uint8_t length, const std::map<uint8_t, Pattern> &map)
{
    int8_t fit = pat.length - length;
    Pattern result = none;

    if (fit < 0)
        misc::print_and_quit("cutout pattern is too smol");
    for (int8_t i = 0; i < fit + 1; i++)
    {
        auto iter = map.find(uint8_t(pat.pattern << (8 - pat.length + i)) >> (fit + (8 - pat.length)));
        if (iter != map.end() && iter->second > result)
        {
            result = iter->second;
            if (result == five || result == open4)
                return result;
        }
    }
    return result;
}

Pattern heuristic::get_heuristic_data(Board &board, const int &move, const int &direction, const int &player, std::bitset<BOARDSIZE> *checked_indices)
{
    Pattern result = none;

    t_pattern pat = get_pattern_data(board, move, direction, player, checked_indices);

    // PRINT((int)pat.count);
    // PRINT((int)pat.left_right[0]);
    // PRINT((int)pat.left_right[1]);
    // PRINT((int)pat.length);
    // PRINT((int)pat.space);
    // PRINT(std::bitset<8>(pat.pattern));

    if (pat.count <= 1 || pat.space < 5)
        return none;
    
    if (pat.count == 2)
    {
        cutout_pattern(board, move, direction, player, pat);
        // PRINT('\n' << std::bitset<8>(pat.pattern));
        if (pat.space > 5 && ((pat.pattern == 0b00000110 && pat.length == 4) || (pat.pattern == 0b00001010 && pat.length == 5))) // .xx. with > 5 space OR .x.x. with > 5 space
            return open2;
        else
            return closed2;
    }
    cutout_pattern(board, move, direction, player, pat);
    if (pat.space == 5 && pat.count == 3) // |x.x.x| & |.xxx.| & |.xx.x| etc.
            return closed3;
    if (pat.length >= 5)
        result = find_subpattern(pat, 5, SUBPATTERNS_5);
    if (result != five && pat.length >= 6)
        result = std::max(result, find_subpattern(pat, 6, SUBPATTERNS_6));
    if (result == none) // xxx. & .xxx & xx.x.x & x.x.x.x & x.x.x. & x.x.x
        return closed3;
    return result;
}

void    heuristic::get_heuristic_single(Board &board, int move, std::bitset<BOARDSIZE> *checked_indices)
{
    int player = board.get_player(move);

    for (int dir = 0; dir < 4; dir++) // four directions
    {
        if (checked_indices[dir][move] == 1)
            continue;
        Pattern pattern = get_heuristic_data(board, move, dir, player, checked_indices);
        // PRINT(PatternNames[pattern]);
        board.players[player].patterns[pattern] += 1;
    }
}

void  heuristic::reset_pattern_arrays(Board &board)
{
    // board.score = 0;
    for (int i = 0; i < 2; i++)
    {
        for (int index = 0; index < 8; index++)
            board.players[i].patterns[index] = 0;
    }
}

int   heuristic::get_heuristic_total(Board &board)
{
    reset_pattern_arrays(board);
    std::bitset<BOARDSIZE> checked_indices[4] = {0, 0, 0, 0};

    // first check the patterns at last_move just to be sure it is correctly registered
    get_heuristic_single(board, board.get_last_move(), checked_indices);
    // if (board.players[player].)
    for (int pos = 0; pos < BOARDSIZE; pos++)
    {
        if (board.filled_pos[pos])
            get_heuristic_single(board, pos, checked_indices);
    }
    return evaluate_patterns(board, board.get_last_player());
}

int				heuristic::count_direction(const Board &board, int index, int player, int dir, int size, std::bitset<BOARDSIZE> &checked_indices)
{
	int length = 0;
	int prev_index;

	for (int i = 0; i < size - 1; i++)
	{
		prev_index = index;
		index += dir;
		if (misc::is_offside(prev_index, index) || board.get_player(index) != player)
			break ;
		checked_indices[index] = 1;
		length++;
	}
	return length;
}

int				heuristic::count_direction(const Board &board, int index, int player, int dir, int size)
{
	int length = 0;
	int prev_index;

	for (int i = 0; i < size - 1; i++)
	{
		prev_index = index;
		index += dir;
		if (misc::is_offside(prev_index, index) || board.get_player(index) != player)
			break ;
		length++;
	}
	return length;
}

int				heuristic::count_both_dir(const Board &board, int index, int player, int dir, std::bitset<BOARDSIZE> &checked_indices)
{
	int total = 1;

	total += count_direction(board, index, player, -dir, 5, checked_indices);
	total += count_direction(board, index, player, dir, 5, checked_indices);
	return total;
}

int				heuristic::count_both_dir(const Board &board, int index, int player, int dir)
{
	int total = 1;

	total += count_direction(board, index, player, -dir, 5);
	total += count_direction(board, index, player, dir, 5);
	return total;
}

// int				heuristic::eight_directions_heuristic(Board &board, int index, std::bitset<BOARDSIZE> &checked_indices, int player)
// {
// 	int points = 0;


// 	points += POINTS[count_both_dir(board, index, player, RIGHT, checked_indices)];
// 	points += POINTS[count_both_dir(board, index, player, DIAGDWNR, checked_indices)];
// 	points += POINTS[count_both_dir(board, index, player, DOWN, checked_indices)];
// 	points += POINTS[count_both_dir(board, index, player, DIAGDWNL, checked_indices)];

// 	// std::cout << (player * points) << std::endl;
//     return player == PLAYER1 ? points : -points;
// }

// int				heuristic::calc_heuristic(Board &board)
// {
// 	std::bitset<BOARDSIZE> checked_indices = 0;
// 	int total_score = 0;
// 	int	player;

// 	for (int index = 0; index < board.filled_pos.size(); index++)
// 	{
// 		if (board.is_empty_place(index))
// 			continue;
// 		if (checked_indices[index])
// 			continue;
// 		player = board.get_player(index);
// 		if (player == 0)
// 			std::cout << index << std::endl;
// 		total_score += eight_directions_heuristic(board, index, checked_indices, player);
// 		checked_indices[index] = 1;
// 	}
// 	if (board.filled_pos.count() != checked_indices.count())
// 	{
// 		board.print();
// 		for (int i = 0; i < board.filled_pos.size(); i++)
// 		{
// 			if (board.is_empty_place(i))
// 				continue;
// 			std::cout << i << " ";
// 		}
// 		std::cout << std::endl << "^ filled positions. check_indices v" << std::endl;
// 		for (int j = 0; j < BOARDSIZE; j++)
// 		{
// 			if (checked_indices[j])
// 				std::cout << j << " ";
// 		}
// 		std::cout << std::endl;
// 	}
// 	return total_score;
// }
