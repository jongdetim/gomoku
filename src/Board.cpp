#include "Board.hpp"

Board::Board(void)
{
	this->state.reset();
}

Board::~Board() {}

void					Board::reset(void)
{
	this->state.reset();
}

std::bitset<MASKSIZE>	Board::get_state(void) const
{
	return this->state;
}

void					Board::print(void)
{
	std::cout << "   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8" << std::endl;
	for (int row = 0; row < BOARDSIZE; row++)
	{
		std::cout << row%10 << ": ";
		for (int col = 0; col < BOARDSIZE; col++)
		{
			int index = (row * BOARDSIZE + col);
			if (this->is_empty_place(index))
				std::cout << ". ";
			else if (this->state[index<<1])
				std::cout << 'o' << ' ';
			else
				std::cout << 'x' << ' ';
		}
		std::cout << std::endl;
	}
}

bool					Board::place(int row, int col, int player)
{
	assert(player == PLAYER1 or player == PLAYER2);
	assert(row < BOARDSIZE and row >= 0 and col < BOARDSIZE and col >= 0);
	int index = (row * BOARDSIZE + col);
	if (not this->is_empty_place(index))
		return false;
	index <<= 1;
	index = player == -1 ? index : index + 1;
	this->state[index] = true;
	return true;
}

bool					Board::place(int index, int player)
{
	int orig_index = index;
	assert(player == PLAYER1 or player == PLAYER2);
	assert(index >= 0 and index < (BOARDSIZE*BOARDSIZE));
	if (not this->is_empty_place(index))
		return false;
	index *= 2;
	index = player == -1 ? index : index + 1;
	this->state[index] = true;
	this->last_move = orig_index;
	return true;
}

bool			Board::is_game_finished()
{
	return false;
}

int				Board::get_heuristic()
{
	return rand() % 1000;
}

// creates a set of positions surrounding the currently occupied spaces
std::unordered_set<int> Board::get_moves(std::vector<int> filled_positions)
{
	std::unordered_set<int> moves;

	for (int index : filled_positions)
	{
		for (int i = 0; i < 8; i++)
		{
			int n_index = index + NEIGHBOURS[i];
			if (0 <= n_index && n_index < BOARDSIZE*BOARDSIZE && is_empty_place(n_index))
			{
				if ((i == 0 || i == 2) && n_index / 19 != (index / 19) - 1)
					continue;
				else if ((i == 5 || i == 7) && n_index / 19 != index / 19 + 1)
					continue;
				else if ((i == 3 || i == 4) && n_index / 19 != index / 19)
					continue;
				moves.insert(n_index);
			}
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

void					Board::remove(int row, int col)
{
	int index = (row * BOARDSIZE + col) << 1;
	this->state[index] = false;
	this->state[index+1] = false;
	
}

void					Board::remove(int index)
{
	index <<= 1;
	this->state[index] = false;
	this->state[index+1] = false;
}

bool					Board::is_empty_place(int index)
{
	index <<= 1;
	return (this->state[index] == false and this->state[index+1] == false);
}

bool 					Board::operator==(Board const &rhs) const
{
	return (this->state == rhs.state);
}

bool 					Board::operator!=(Board const &rhs) const
{
	return (this->state != rhs.state);
}

bool 					Board::operator==(int const rhs) const
{
	return (this->state == rhs);
}

bool 					Board::operator!=(int const rhs) const
{
	return (this->state != rhs);
}

std::bitset<MASKSIZE>	Board::operator&(Board const &rhs) const
{
	return this->state & rhs.state;
}

std::bitset<MASKSIZE>	Board::operator&(std::bitset<MASKSIZE> const &rhs) const
{
	return this->state & rhs;
}

std::ostream			&operator<<(std::ostream &o, Board const &i)
{
	o << i.get_state();
	return o;
}
