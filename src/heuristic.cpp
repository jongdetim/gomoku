#include "heuristic.hpp"
#include "misc.hpp"

// #define PRINT(x) std::cout << x << std::endl
// #define PRINTENDL std::cout << std::endl

// int						get_hor_pattern(Board &board, int start_index, int player) // <---- TESTING
// {
// 	int pattern = 0;
// 	int col = get_col(start_index);
// 	int leftside = col < 4 ?  col : 4;
// 	int offset = (BOARD_LENGHT-1) - col;
// 	int rightside = offset < 4 ?  offset : 4;
// 	int total = leftside + rightside + 1;
// 	int index = start_index - leftside;

// 	for (int i = 0; i < total; i++)
// 	{
// 		if (board.get_player(index) == -player)
// 		{
// 			if (index < start_index)
// 				pattern = 0;
// 			else
// 				break ;checked_indices.insert(index);
// 		}
// 		else if (board.get_player(index) == player)
// 			pattern = (pattern << 1) ^ 1;
// 		else
// 			pattern <<= 1;
// 		index++;
// 	}
// 	return pattern;
// }
////////////////////////////////////////////////////////////
int						g_points[6]{0,0,ROW2,ROW3,ROW4,ROW5};

static int				go_down(Board &node, int index, int player, std::unordered_set<int> &checked_indices)
{
	int length = 0;

	for (int i = 0; i < 4; i++)
	{
		index += BOARD_LENGHT;
		if (index >= BOARDSIZE || node.get_player(index) != player)
			break ;
		checked_indices.insert(index);
		length++;
	}
	return length;
}

static int				go_up(Board &node, int index, int player, std::unordered_set<int> &checked_indices)
{
	int length = 0;

	for (int i = 0; i < 4; i++)
	{
		index -= BOARD_LENGHT;
		if (index < 0 || node.get_player(index) != player)
			break ;
		checked_indices.insert(index);
		length++;
	}
	return length;
}

int						count_ver(Board &node, int index, int player, std::unordered_set<int> &checked_indices)
{
	int total = 1;

	total += go_up(node, index, player, checked_indices);
	total += go_down(node, index, player, checked_indices);
	// std::cout << "VER:       " << total << " points: " << g_points[total] << std::endl;
	return total;
}

static int				go_left(Board &node, int index, int player, std::unordered_set<int> &checked_indices)
{
	int length = 0;
	int col = get_col(index);
	int offside = col < 4 ?  col : 4;

	for (int i = 0; i < offside; i++)
	{
		index--;
		if (node.get_player(index) != player)
			break ;
		checked_indices.insert(index);
		length++;
		
	}
	return length;
}

static int				go_right(Board &node, int index, int player, std::unordered_set<int> &checked_indices)
{
	int length = 0;
	int offset = (BOARD_LENGHT-1) - get_col(index);
	int offside = offset < 4 ?  offset : 4;

	for (int i = 0; i < offside; i++)
	{
		index++;
		if (node.get_player(index) != player)
			break ;
		checked_indices.insert(index);
		length++;
		
	}
	return length;
}

int						count_hor(Board &node, int index, int player, std::unordered_set<int> &checked_indices)
{
	int total = 1;

	total += go_left(node, index, player, checked_indices);
	total += go_right(node, index, player, checked_indices);
	// std::cout << "HOR:       " << total  << " points: " << g_points[total] << std::endl;
	return total;
}

static int				diag_upR(Board &node, int index, int player, std::unordered_set<int> &checked_indices)
{
	int length = 0;
	int offset = (BOARD_LENGHT-1) - get_col(index);
	int offside = offset < 4 ?  offset : 4;

	for (int i = 0; i < offside; i++)
	{
		index -= (BOARD_LENGHT -1);
		if (index < 0 || node.get_player(index) != player)
			break ;
		checked_indices.insert(index);
		length++;
		
	}
	return length;
}

static int				diag_downL(Board &node, int index, int player, std::unordered_set<int> &checked_indices)
{
	int length = 0;
	int col = get_col(index);
	int offside = col < 4 ?  col : 4;

	for (int i = 0; i < offside; i++)
	{
		index += (BOARD_LENGHT-1);
		if (index >= BOARDSIZE || node.get_player(index) != player)
			break ;
		checked_indices.insert(index);
		length++;
		
	}
	return length;
}

int						count_diag_up(Board &node, int index, int player, std::unordered_set<int> &checked_indices)
{
	int total = 1;

	total += diag_upR(node, index, player, checked_indices);
	total += diag_downL(node, index, player, checked_indices);
	// std::cout << "DIAG_UP:   " << total  << " points: " << g_points[total] << std::endl;
	return total;
}

static int				diag_upL(Board &node, int index, int player, std::unordered_set<int> &checked_indices)
{
	int length = 0;
	int col = get_col(index);
	int offside = col < 4 ?  col : 4;

	for (int i = 0; i < offside; i++)
	{
		index -= (BOARD_LENGHT+1);
		if (index < 0 || node.get_player(index) != player)
			break ;
		checked_indices.insert(index);
		
		length++;
	}
	return length;
}

static int				diag_downR(Board &node, int index, int player, std::unordered_set<int> &checked_indices)
{
	int length = 0;
	int offset = (BOARD_LENGHT-1) - get_col(index);
	int offside = offset < 4 ?  offset : 4;

	for (int i = 0; i < offside; i++)
	{
		index += (BOARD_LENGHT+1);
		if (index >= BOARDSIZE || node.get_player(index) != player)
			break ;
		checked_indices.insert(index);
		
		length++;
	}
	return length;
}

int						count_diag_down(Board &node, int index, int player, std::unordered_set<int> &checked_indices)
{
	int total = 1;

	total += diag_upL(node, index, player, checked_indices);
	total += diag_downR(node, index, player, checked_indices);
	// std::cout << "DIAG_DOWN: " << total  << " points: " << g_points[total] << std::endl;
	return total;
}

// int						get_heuristic_last_move(Board &board)
// {
// 	int points = 0;

// 	points += g_points[count_hor(board, board.last_move, board.get_last_player())];
// 	points += g_points[count_ver(board, board.last_move, board.get_last_player())];
// 	points += g_points[count_diag_down(board, board.last_move, board.get_last_player())];
// 	points += g_points[count_diag_up(board, board.last_move, board.get_last_player())];

// 	return points;
// }

int						calc_heuristic(Board &state)
{
	int points = 0;

	return points;
}

int		eight_directions_heuristic(int index, std::unordered_set<int> &checked_indices, int player, Board &node)
{
	int points = 0;

	points += g_points[count_hor(node, index, player, checked_indices)];
	points += g_points[count_ver(node, index, player, checked_indices)];
	points += g_points[count_diag_down(node, index, player, checked_indices)];
	points += g_points[count_diag_up(node, index, player, checked_indices)];

	return player * points;
}

int		calc_heuristic_tim(std::vector<int> filled_positions, Board &node)
{
	std::unordered_set<int> checked_indices;
	int total_score = 0;
	int	player = 0;

	for (int index : filled_positions)
	{
		if (checked_indices.find(index) != checked_indices.end())
			continue;
		player = node.get_player(index);
		total_score += eight_directions_heuristic(index, checked_indices, player, node);
		checked_indices.insert(index);
	}
	// std::cout << filled_positions.size() << " " << total_score << std::endl;
	return total_score;
}
