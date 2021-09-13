#include "Board.hpp"
#include <iostream>
#include <cassert>

Board::Board(void)
{
	this->state.reset();
}

Board::~Board() {}

void			Board::print(void)
{
	std::cout << "   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8" << std::endl;
	for (int row = 0; row < BOARDSIZE; row++)
	{
		std::cout << row%10 << ": ";
		for (int col = 0; col < BOARDSIZE; col++)
		{
			int index = (row * BOARDSIZE + col);
			if (this->is_empty(index))
				std::cout << ". ";
			else if (this->state[index * 2])
				std::cout << 'o' << ' ';
			else
				std::cout << 'x' << ' ';
		}
		std::cout << std::endl;
	}
}

bool			Board::place(int row, int col, int player)
{
	assert(player == PLAYER1 or player == PLAYER2);
	assert(row < BOARDSIZE and row >= 0 and col < BOARDSIZE and col >= 0);
	int index = (row * BOARDSIZE + col);
	if (not this->is_empty(index))
		return false;
	index *= 2;
	index = player == -1 ? index : index + 1;
	this->state[index] = true;
	this->last_move = (row * 19) + col;
	return true;
}

bool			Board::place(int index, int player)
{
	int orig_index = index;
	assert(player == PLAYER1 or player == PLAYER2);
	assert(index >= 0 and index < (BOARDSIZE*BOARDSIZE));
	if (not this->is_empty(index))
		return false;
	index *= 2;
	index = player == -1 ? index : index + 1;
	this->state[index] = true;
	this->last_move = orig_index;
	return true;
}

bool		Board::is_empty(int index)
{
	index *= 2;
	return (this->state[index] == false and this->state[index+1] == false);
}

bool			Board::is_game_finished()
{
	return false;
}

int				Board::get_heuristic()
{
	return 0;
}

// creates a set of positions surrounding the currently occupied spaces
std::unordered_set<int> Board::get_moves(std::vector<int> filled_positions)
{
	std::unordered_set<int> moves;

	for (int index : filled_positions)
	{
		// dit checkt nog niet voor edges van het bord
		for (int neighbour : neighbours)
		{
			int i = index + neighbour;
			if (0 <= i && i < 361 && is_empty(i))
				moves.insert(i);
		}
	}
	return moves;
}

std::vector<Board> Board::generate_children(std::vector<int> filled_positions, int player)
{
	Board board_copy;
    std::vector<Board> nodes;
	std::unordered_set<int> moves;

	moves = get_moves(filled_positions);
	for (int move : moves)
	{
		board_copy = *this;
		board_copy.place(move, player);
		nodes.push_back(board_copy);
	}
    return nodes;
}
