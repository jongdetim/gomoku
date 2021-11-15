#include "Board.hpp"
#include "misc.hpp"

Board::Board(void) : last_move(-1), h(0), state(0), stones_played(0) {}

Board::~Board() {}

void					Board::reset(void)
{
	this->stones_played = 0;
	this->state.reset();
}

BITBOARD				Board::get_state(void) const
{
	return this->state;
}

void					Board::print(void) const
{
	std::cout << "   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8" << std::endl;
	for (int row = 0; row < BOARD_LENGHT; row++)
	{
		std::cout << row%10 << ": ";
		for (int col = 0; col < BOARD_LENGHT; col++)
		{
			int index = (row * BOARD_LENGHT + col);
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

void					Board::show_last_move(void) const
{
	std::cout << "   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8" << std::endl;
	for (int row = 0; row < BOARD_LENGHT; row++)
	{
		std::cout << row%10 << ": ";
		for (int col = 0; col < BOARD_LENGHT; col++)
		{
			int index = (row * BOARD_LENGHT + col);
			if (this->is_empty_place(index))
				std::cout << ". ";
			else if (this->state[index<<1])
			{
				if (this->last_move == index)
					printf("\x1B[33mO \033[0m");
				else
					std::cout << 'o' << ' ';
			}
			else
			{
				if (this->last_move == index)
					printf("\x1B[33mX \033[0m");
				else
					std::cout << 'x' << ' ';
			}
		}
		std::cout << std::endl;
	}
}

bool					Board::place(int row, int col, int player)
{
	assert(player == PLAYER1 || player == PLAYER2);
	assert(row < BOARD_LENGHT && row >= 0 && col < BOARD_LENGHT && col >= 0);
	int index = (row * BOARD_LENGHT + col);
	int orig_index = index;
	if (! this->is_empty_place(index))
		return false;
	index <<= 1;
	index = player == PLAYER1 ? index : index + 1;
	this->state[index] = true;
	this->last_move = orig_index;
	this->stones_played++;
	return true;
}

bool					Board::place(int index, int player)
{
	int orig_index = index;
	assert(player == PLAYER1 || player == PLAYER2);
	assert(index >= 0 && index < (BOARD_LENGHT*BOARD_LENGHT));
	if (! this->is_empty_place(index))
		return false;
	index *= 2;
	index = player == PLAYER1 ? index : index + 1;
	this->state[index] = true;
	this->last_move = orig_index;
	this->stones_played++;
	return true;
}

bool					Board::free_threes_direction(int move, int direction, int player) const
{
	int pos;
	int gaps = 0;
	int count = 0;
	int open = 0;
	int shift;
	for (int j = 0; j < 2; j++)
	{
		pos = move;
		shift = NEIGHBOURS[direction + 4 * j];
		if (gaps > 1)
			return false;
		for (int i = 0; i < 4; i++)
		{
			pos += shift;
			if (is_offside(pos - shift, pos))
				break;
			if (get_player(pos) == player)
				count++;
			else if (get_player(pos) == -player)
				break;
			else if (!(is_offside(pos, pos + shift)) && get_player(pos + shift) == player)
				gaps++;
			else
				open++;
		}
	}
	return count == 2 && gaps < 2 && open + gaps > 2;
}

bool					Board::check_free_threes(int move, int player) const
{
	int result = 0;
	// still need to check if last move was NOT a capture
	for (int i = 0; i < 4; i++)
	{
		result += free_threes_direction(move, i, player);
		if (result > 1)
			break;
	}
	return result > 1;
}

bool					Board::is_game_finished() const
{
	return false;
}

int						Board::get_random_heuristic() const
{
	return (rand() % 1000) - 500;
}

// creates a set of positions surrounding the currently occupied spaces
std::set<int> Board::get_moves(std::vector<int> &filled_positions) const
{
	//  dit is sneller met een bitset maar waarom komt het tot een ander resultaat?
	std::set<int> moves;

	for (int index : filled_positions)
	{
		for (int i = 0; i < 8; i++)
		{
			int n_index = index + NEIGHBOURS[i];
			if (0 <= n_index && n_index < BOARDSIZE && is_empty_place(n_index))
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

std::vector<int> Board::get_moves_vect(std::vector<int> &filled_positions) const
{
	//  dit is sneller met een bitset maar waarom komt het tot een ander resultaat?
	std::vector<int> moves;

	for (int index : filled_positions)
	{
		for (int i = 0; i < 8; i++)
		{
			int n_index = index + NEIGHBOURS[i];
			if (0 <= n_index && n_index < BOARDSIZE && is_empty_place(n_index))
			{
				if ((i == 0 || i == 2) && n_index / 19 != (index / 19) - 1)
					continue;
				else if ((i == 5 || i == 7) && n_index / 19 != index / 19 + 1)
					continue;
				else if ((i == 3 || i == 4) && n_index / 19 != index / 19)
					continue;
				moves.push_back(n_index);
			}
		}
	}
	std::sort( moves.begin(), moves.end() );
	moves.erase( std::unique( moves.begin(), moves.end() ), moves.end() );
	return moves;
}

void Board::get_moves_bits(std::vector<int> &filled_positions, std::bitset<BOARDSIZE> &moves, int player)
{
	for (int index : filled_positions)
	{
		for (int i = 0; i < 8; i++)
		{
			int n_index = index + NEIGHBOURS[i];
			if (0 <= n_index && n_index < BOARDSIZE && is_empty_place(n_index))
			{
				if ((i == 0 || i == 2) && n_index / 19 != (index / 19) - 1)
					continue;
				else if ((i == 5 || i == 7) && n_index / 19 != index / 19 + 1)
					continue;
				else if ((i == 3 || i == 4) && n_index / 19 != index / 19)
					continue;
				// else if (check_free_threes(n_index, player))
				// 	continue;
				moves[n_index] = 1;
			}
		}
	}
}

std::vector<Board>		Board::generate_children(std::vector<int> &filled_positions, int player) const
{
	Board board_copy;
    std::vector<Board> nodes;
	std::set<int> moves;

	moves = get_moves(filled_positions);
	for (int move : moves)
	{
		board_copy = *this;
		board_copy.place(move, player);
		nodes.push_back(board_copy);
		// de volgorde hier heeft invloed op de search, ondanks dat deze children nodes nog worden resorteerd. komt dit door gelijke heuristic values en pruning?
		// nodes.insert(nodes.begin(), board_copy);
	}
    return nodes;
}

std::vector<Board>		Board::generate_children_vect(std::vector<int> &filled_positions, int player) const
{
	Board board_copy;
    std::vector<Board> nodes;
	std::vector<int> moves;

	moves = get_moves_vect(filled_positions);
	for (int move : moves)
	{
		board_copy = *this;
		board_copy.place(move, player);
		nodes.push_back(board_copy);
		// de volgorde hier heeft invloed op de search, ondanks dat deze children nodes nog worden resorteerd. komt dit door gelijke heuristic values en pruning?
		// nodes.insert(nodes.begin(), board_copy);
	}
    return nodes;
}

std::vector<Board>		Board::generate_children_bits(std::vector<int> &filled_positions, int player)
{
	// Board board_copy;
    std::vector<Board> nodes;
	std::bitset<BOARDSIZE> moves;

	get_moves_bits(filled_positions, moves, player);
	for (int i = 0; i < BOARDSIZE; i++)
	{
		if (moves[i])
		{
			// board_copy = *this;
			(*this).place(i, player);
			nodes.push_back(*this);
			(*this).remove(i);
			// de volgorde hier heeft invloed op de search, ondanks dat deze children nodes nog worden resorteerd. komt dit door gelijke heuristic values en pruning?
			// nodes.insert(nodes.begin(), board_copy);
		}
	}
    return nodes;
}

void					Board::remove(int row, int col)
{
	int index = (row * BOARD_LENGHT + col);
	if (this->is_empty_place(index))
		return ;
	index <<= 1;
	this->state[index] = false;
	this->state[index+1] = false;
	this->stones_played--;
	
}

void					Board::remove(int index)
{
	if (this->is_empty_place(index))
		return ;
	index <<= 1;
	this->state[index] = false;
	this->state[index+1] = false;
	this->stones_played--;
}

bool					Board::is_empty_place(int index) const
{
	index <<= 1;
	return (this->state[index] == false && this->state[index+1] == false);
}

int						Board::get_player(int index) const
{
	index <<= 1;
	if (this->state[index])
		return PLAYER1;
	else if (this->state[index+1])
		return PLAYER2;
	return 0;
}

int						Board::get_last_player(void) const
{
	return this->get_player(this->last_move);
}

bool					Board::is_full(void) const
{
	return (this->stones_played == BOARDSIZE);
}

void					Board::set_state(BITBOARD new_state)
{
	this->state = new_state;
}

/* OPERATOR OVERLOADS: */

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

BITBOARD				Board::operator&(Board const &rhs) const
{
	return this->state & rhs.state;
}

BITBOARD				Board::operator&(BITBOARD const &rhs) const
{
	return this->state & rhs;
}

std::ostream			&operator<<(std::ostream &o, Board const &i)
{
	o << i.get_state();
	return o;
}
