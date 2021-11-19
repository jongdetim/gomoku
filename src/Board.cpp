#include "Board.hpp"
#include "misc.hpp"

Board::Board(void) : h(0), state(0), stones_in_play(0), last_move(-1), filled_pos(0)
{
	this->player1 = t_player{0};
	this->player2 = t_player{0};
}

Board::~Board() {}

void					Board::reset(void)
{
	this->h = 0;
	this->state.reset();
	this->last_move = -1;
	this->stones_in_play = 0;
	this->player1 = t_player{0};
	this->player2 = t_player{0};
	this->filled_pos.reset();
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

int						Board::get_player_index(int index, int player) const { return player == PLAYER1 ? index << 1 : (index << 1) + 1; }

bool					Board::place(int row, int col, int player) { return this->place(this->calculate_index(row, col), player); }

bool					Board::place(int index, int player)
{
	assert(player == PLAYER1 || player == PLAYER2);
	assert(index >= 0 && index < BOARDSIZE);

	if (!this->is_valid_move(index, player))
		return false;
	
	this->filled_pos[index] = 1;
	this->last_move = index;

	this->state[this->get_player_index(index, player)] = 1;
	this->stones_in_play++;

	this->update_player(player, this->check_captures(player, index));
	return true;
}

bool					Board::is_game_won(void) const { return this->heuristic.has_won(this, this->last_move, this->get_last_player()); }

bool					Board::is_game_won(int index, int player) const { return this->heuristic.has_won(this, index, player); }

bool					Board::is_game_finished(void) const { return this->is_full(); }

std::vector<Board>		Board::generate_children(int player) const
{
	Board board_copy;
    std::vector<Board> nodes;

	auto moves = get_moves();
	for (int i = 0; i < moves.size(); i++)
	{
		if (!moves[i])
			continue;
		board_copy = *this;
		board_copy.place(i, player);
		nodes.push_back(board_copy);
		// de volgorde hier heeft invloed op de search, ondanks dat deze children nodes nog worden resorteerd. komt dit door gelijke heuristic values en pruning?
		// nodes.insert(nodes.begin(), board_copy);
	}
    return nodes;
}

void					Board::remove(int row, int col) { this->remove(this->calculate_index(row, col)); }

void					Board::remove(int index)
{
	if (this->is_empty_place(index))
		return ;
	this->filled_pos[index] = 0;
	index <<= 1;
	this->state[index] = 0;
	this->state[index+1] = 0;
	this->stones_in_play--;
}

bool					Board::is_empty_place(int index) const { return this->filled_pos[index] == 0; }

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

bool					Board::is_full(void) const { return (this->stones_in_play == BOARDSIZE); }

void					Board::set_state(BITBOARD new_state) { this->state = new_state; }

int						Board::calculate_index(int row, int col) const { return (row * BOARD_LENGTH + col); }

int						Board::get_stones_in_play(void) const { return this->stones_in_play; }

int						Board::get_player_captures(int player) const { return player == PLAYER1 ? this->player1.captures : this->player2.captures; }

int						Board::check_captures(int player, int index)
{
	int amount = 0;

	for (auto dir : DIRECTIONS)
	{
		if (this->can_capture(player, index, dir))
		{
			this->capture(dir, index);
			amount++;
		}
	}
	return amount;
}

/* PRIVATE METHODS */

// Assumes that on the given index the correct player is placed
bool					Board::can_capture(int player, int index, int dir) const
{
	for (int i = 1; i < 4; i++)
	{
		if (is_offside(index, index + dir))
			break ;
		index += dir;
		if (i == 3 && this->get_player(index) == player)
			return true;
		else if (this->get_player(index) != -player)
			break ;
	}
	return false;
}

void					Board::capture(int dir, int index)
{
	for (int i = 1; i < 3; i++)
		this->remove(index + (i * dir));
}

void					Board::update_player(int player, int captures)
{
	if (player == PLAYER1)
		this->player1.captures += captures;
	else if (player == PLAYER2)
		this->player2.captures += captures;
}

bool					Board::is_valid_move(int index, int player) const
{
	if (!this->is_empty_place(index))
		return false;
	return true;
}

// creates a set of positions surrounding the currently occupied spaces
std::bitset<BOARDSIZE>	Board::get_moves(void) const
{
	std::bitset<BOARDSIZE> moves{0};

	for (int index = 0; index < this->filled_pos.size(); index++)
	{
		if (!this->filled_pos[index])
			continue ;
		for (int i = 0; i < 8; i++)
		{
			int n_index = index + DIRECTIONS[i];
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
	return moves;
}

/* OPERATOR OVERLOADS: */

bool 					Board::operator==(Board const &rhs) const { return (this->state == rhs.state); }

bool 					Board::operator!=(Board const &rhs) const { return (this->state != rhs.state); }

bool 					Board::operator==(int const rhs) const { return (this->state == rhs); }

bool 					Board::operator!=(int const rhs) const { return (this->state != rhs); }

BITBOARD				Board::operator&(Board const &rhs) const { return this->state & rhs.state; }

BITBOARD				Board::operator&(BITBOARD const &rhs) const { return this->state & rhs; }

std::ostream			&operator<<(std::ostream &o, Board const &i) { return (o << i.get_state()); }
