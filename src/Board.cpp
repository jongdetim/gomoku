#include "Board.hpp"
#include "misc.hpp"

Board::Board(void) : h(0), state(0), stones_played(0), last_move(-1)
{
	this->player1 = t_player{0,0};
	this->player2 = t_player{0,0};
}

Board::~Board() {}

void					Board::reset(void)
{
	this->h = 0;
	this->state.reset();
	this->last_move = -1;
	this->stones_played = 0;
	this->player1 = t_player{0,0};
	this->player2 = t_player{0,0};
}

BITBOARD				Board::get_state(void) const { return this->state; }

int						Board::get_last_move(void) const { return this->last_move; }

void					Board::print(void) const
{
	std::cout << "   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8" << std::endl;
	for (int row = 0; row < BOARD_LENGTH; row++)
	{
		std::cout << row%10 << ": ";
		for (int col = 0; col < BOARD_LENGTH; col++)
		{
			int index = (row * BOARD_LENGTH + col);
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
	for (int row = 0; row < BOARD_LENGTH; row++)
	{
		std::cout << row%10 << ": ";
		for (int col = 0; col < BOARD_LENGTH; col++)
		{
			int index = (row * BOARD_LENGTH + col);
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

bool					Board::place(int row, int col, int player) { return this->place(this->calculate_index(row, col), player); }

bool					Board::place(int index, int player)
{
	assert(player == PLAYER1 || player == PLAYER2);
	assert(index >= 0 && index < BOARDSIZE);

	int orig_index = index;
	if (!this->is_valid_move(index, player))
		return false;
	index <<= 1;
	index = player == PLAYER1 ? index : index + 1;
	this->state[index] = true;
	this->last_move = orig_index;
	this->stones_played++;
	this->check_capture();
	return true;
}

bool					Board::is_game_won(void) const { return this->heuristic.check_win(this); }

bool					Board::is_game_finished(void) const { return this->is_full(); }

void					Board::capture(int dir, int index)
{
	for (int i = 1; i < 3; i++)
		this->remove(index + (i * dir));
}

void					Board::check_capture(void)
{
	int directions[8] {RIGHT, DIAGDWNR, DOWN, DIAGDWNL, LEFT, DIAGUPL, UP, DIAGUPR};
	bool capture;
	int index, player = this->get_last_player();

	for (auto dir : directions)
	{
		capture = false;
		index = this->last_move;
		for (int i = 1; i < 4; i++)
		{
			index += dir;
			if (is_offside(index - dir, index) || index < 0 || index >= BOARDSIZE)
				break ;
			if (i == 3 && this->get_player(index) == player)
				capture = true;
			else if (this->get_player(index) != -player)
				break ;
		}
		if (capture)
			this->capture(dir, this->last_move);
	}
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

void Board::get_moves_bits(std::vector<int> &filled_positions, std::bitset<BOARDSIZE> &moves)
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
	Board board_copy;
    std::vector<Board> nodes;
	std::bitset<BOARDSIZE> moves;

	get_moves_bits(filled_positions, moves);
	for (int i = 0; i < BOARDSIZE; i++)
	{
		if (moves[i])
		{
			board_copy = *this;
			board_copy.place(i, player);
			nodes.push_back(board_copy);
			// de volgorde hier heeft invloed op de search, ondanks dat deze children nodes nog worden resorteerd. komt dit door gelijke heuristic values en pruning?
			// nodes.insert(nodes.begin(), board_copy);
		}
	}
    return nodes;
}

void					Board::remove(int row, int col) { this->remove(this->calculate_index(row, col)); }

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

int						Board::get_last_player(void) const { return this->get_player(this->last_move); }

bool					Board::is_full(void) const { return (this->stones_played == BOARDSIZE); }

void					Board::set_state(BITBOARD new_state) { this->state = new_state; }

int						Board::calculate_index(int row, int col) const { return (row * BOARD_LENGTH + col); }

int						Board::get_captures(int player) const { return player == PLAYER1 ? this->player1.captures : this->player2.captures; }

/* PRIVATE METHODS */

void					Board::update_player(int player, int captures)
{
	if (player == PLAYER1)
	{
		this->player1.stones_played++;
		this->player1.captures += captures;

		this->player2.stones_played -= (captures << 1);
	}
	else if (player == PLAYER2)
	{
		this->player2.stones_played++;
		this->player2.captures += captures;

		this->player1.stones_played -= (captures << 1);
	}
}

bool					Board::is_valid_move(int index, int player) const
{
	if (!this->is_empty_place(index))
		return false;
	return true;
}

int						Board::get_stones_played(void) const { return this->stones_played; }

/* OPERATOR OVERLOADS: */

bool 					Board::operator==(Board const &rhs) const { return (this->state == rhs.state); }

bool 					Board::operator!=(Board const &rhs) const { return (this->state != rhs.state); }

bool 					Board::operator==(int const rhs) const { return (this->state == rhs); }

bool 					Board::operator!=(int const rhs) const { return (this->state != rhs); }

BITBOARD				Board::operator&(Board const &rhs) const { return this->state & rhs.state; }

BITBOARD				Board::operator&(BITBOARD const &rhs) const { return this->state & rhs; }

std::ostream			&operator<<(std::ostream &o, Board const &i) { return (o << i.get_state()); }
