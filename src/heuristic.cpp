#include "heuristic.hpp"
#include "misc.hpp"

int						g_points[6]{0,0,ROW2,ROW3,ROW4,ROW5};

static int				go_down(Board &node, int index, int player, std::bitset<BOARDSIZE> &checked_indices)
{
	int length = 0;

	for (int i = 0; i < 4; i++)
	{
		index += BOARD_LENGTH;
		if (index >= BOARDSIZE || node.get_player(index) != player)
			break ;
		checked_indices[index] = 1;
		length++;
	}
	return length;
}

static int				go_up(Board &node, int index, int player, std::bitset<BOARDSIZE> &checked_indices)
{
	int length = 0;

	for (int i = 0; i < 4; i++)
	{
		index -= BOARD_LENGTH;
		if (index < 0 || node.get_player(index) != player)
			break ;
		checked_indices[index] = 1;
		length++;
	}
	return length;
}

int						count_ver(Board &node, int index, int player, std::bitset<BOARDSIZE> &checked_indices)
{
	int total = 1;

	total += go_up(node, index, player, checked_indices);
	total += go_down(node, index, player, checked_indices);
	// std::cout << "VER:       " << total << " points: " << g_points[total] << std::endl;
	return total;
}

static int				go_left(Board &node, int index, int player, std::bitset<BOARDSIZE> &checked_indices)
{
	int length = 0;
	int col = get_col(index);
	int offside = col < 4 ?  col : 4;

	for (int i = 0; i < offside; i++)
	{
		index--;
		if (node.get_player(index) != player)
			break ;
		checked_indices[index] = 1;
		length++;
		
	}
	return length;
}

static int				go_right(Board &node, int index, int player, std::bitset<BOARDSIZE> &checked_indices)
{
	int length = 0;
	int offset = (BOARD_LENGTH-1) - get_col(index);
	int offside = offset < 4 ?  offset : 4;

	for (int i = 0; i < offside; i++)
	{
		index++;
		if (node.get_player(index) != player)
			break ;
		checked_indices[index] = 1;
		length++;
		
	}
	return length;
}

int						count_hor(Board &node, int index, int player, std::bitset<BOARDSIZE> &checked_indices)
{
	int total = 1;

	total += go_left(node, index, player, checked_indices);
	total += go_right(node, index, player, checked_indices);
	// std::cout << "HOR:       " << total  << " points: " << g_points[total] << std::endl;
	return total;
}

static int				diag_upR(Board &node, int index, int player, std::bitset<BOARDSIZE> &checked_indices)
{
	int length = 0;
	int offset = (BOARD_LENGTH-1) - get_col(index);
	int offside = offset < 4 ?  offset : 4;

	for (int i = 0; i < offside; i++)
	{
		index -= (BOARD_LENGTH - 1);
		if (index < 0 || node.get_player(index) != player)
			break ;
		checked_indices[index] = 1;
		length++;
	}
	return length;
}

static int				diag_downL(Board &node, int index, int player, std::bitset<BOARDSIZE> &checked_indices)
{
	int length = 0;
	int col = get_col(index);
	int offside = col < 4 ?  col : 4;

	for (int i = 0; i < offside; i++)
	{
		index += (BOARD_LENGTH-1);
		if (index >= BOARDSIZE || node.get_player(index) != player)
			break ;
		checked_indices[index] = 1;
		length++;
		
	}
	return length;
}

int						count_diag_up(Board &node, int index, int player, std::bitset<BOARDSIZE> &checked_indices)
{
	int total = 1;

	total += diag_upR(node, index, player, checked_indices);
	total += diag_downL(node, index, player, checked_indices);
	// std::cout << "DIAG_UP:   " << total  << " points: " << g_points[total] << std::endl;
	return total;
}

static int				diag_upL(Board &node, int index, int player, std::bitset<BOARDSIZE> &checked_indices)
{
	int length = 0;
	int col = get_col(index);
	int offside = col < 4 ?  col : 4;

	for (int i = 0; i < offside; i++)
	{
		index -= (BOARD_LENGTH+1);
		if (index < 0 || node.get_player(index) != player)
			break ;
		checked_indices[index] = 1;
		length++;
	}
	return length;
}

static int				diag_downR(Board &node, int index, int player, std::bitset<BOARDSIZE> &checked_indices)
{
	int length = 0;
	int offset = (BOARD_LENGTH-1) - get_col(index);
	int offside = offset < 4 ?  offset : 4;

	for (int i = 0; i < offside; i++)
	{
		index += (BOARD_LENGTH+1);
		if (index >= BOARDSIZE || node.get_player(index) != player)
			break ;
		checked_indices[index] = 1;
		
		length++;
	}
	return length;
}

int						count_diag_down(Board &node, int index, int player, std::bitset<BOARDSIZE> &checked_indices)
{
	int total = 1;

	total += diag_upL(node, index, player, checked_indices);
	total += diag_downR(node, index, player, checked_indices);
	// std::cout << "DIAG_DOWN: " << total  << " points: " << g_points[total] << std::endl;
	return total;
}

int		eight_directions_heuristic(int index, std::bitset<BOARDSIZE> &checked_indices, int player, Board &node)
{
	int points = 0;

	points += g_points[count_hor(node, index, player, checked_indices)];
	points += g_points[count_ver(node, index, player, checked_indices)];
	points += g_points[count_diag_down(node, index, player, checked_indices)];
	points += g_points[count_diag_up(node, index, player, checked_indices)];

	return points;
}


int		calc_heuristic_tim(std::vector<int> filled_positions, Board &node, bool from_parent)
{
	std::bitset<BOARDSIZE> checked_indices = 0;
	int total_score = 0;
	int	player = 0;

	if (from_parent)
		filled_positions.push_back(node.get_last_move());
	for (int index : filled_positions)
	{
		if (checked_indices[index])
			continue;
		player = node.get_player(index);
		if (player == 0)
			std::cout << index << std::endl;
		total_score += eight_directions_heuristic(index, checked_indices, player, node);
		checked_indices[index] = 1;
	}
	if (filled_positions.size() != checked_indices.count())
	{
		node.print();
		for (int i : filled_positions)
			std::cout << i << " ";
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
