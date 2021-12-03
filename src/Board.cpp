#include "Board.hpp"
#include "misc.hpp"

Board::Board(void) : h(0), state(0), filled_pos(0)
{
	this->set_players();
}

Board::~Board() {}

void					Board::set_players(void)
{
	// PLAYER1 = std::make_shared<t_player>();
	PLAYER1.name = "P1";
	PLAYER1.id = PLAYER1_ID;
	PLAYER1.index_offset = 0;
	PLAYER1.last_move = -1;
	PLAYER1.captures = 0;
	PLAYER1.stones_in_play = 0;
	
	// PLAYER2 = std::make_shared<t_player>();
	PLAYER2.name = "P2";
	PLAYER2.id = PLAYER2_ID;
	PLAYER2.index_offset = 1;
	PLAYER2.last_move = -1;
	PLAYER2.captures = 0;
	PLAYER2.stones_in_play = 0;
	PLAYER2.next = &PLAYER1;

	PLAYER1.next = &PLAYER2;
	PLAYER = &PLAYER1;
}

void					Board::reset_player(Player &player)
{
	player.last_move = -1;
	player.captures = 0;
	player.stones_in_play = 0;
}

void					Board::reset(void)
{
	this->h = 0;
	this->state.reset();
	this->last_move = -1;
	this->reset_player(PLAYER1);
	this->reset_player(PLAYER2);
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
					printf("%sO %s", CYAN, DEFAULT);
				else
					std::cout << 'o' << ' ';
			}
			else
			{
				if (this->last_move == index)
					printf("%sX %s", RED, DEFAULT);
				else
					std::cout << 'x' << ' ';
			}
		}
		std::cout << std::endl;
	}
}

bool					Board::place(int row, int col, int player) { return this->place(calc_index(row, col), player); }

bool					Board::place(int index, int player) { return this->place(index, *this->get_player_by_id(player)); }

bool					Board::place(int index, Player &player)
{
	if (!this->is_valid_move(index, player))
		return false;
	
	this->filled_pos[index] = 1;
	this->last_move = player.last_move = index;

	this->state[(index << 1) + player.index_offset] = 1;
	player.captures = this->check_captures(player.id, index);
	return true;
}

bool					Board::has_won(void) const { return this->heuristic.has_won(this, *PLAYER); }

bool					Board::has_won(Player &player) const { return this->heuristic.has_won(this, player); }

bool					Board::is_game_finished(void) const { return (this->is_full() || this->has_won()); }

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

void					Board::remove(int row, int col) { this->remove(calc_index(row, col)); }

void					Board::remove(int index)
{
	if (this->is_empty_place(index))
		return ;
	this->filled_pos[index] = 0;
	this->get_player_by_id(this->get_player_id(index))->stones_in_play--;
	index <<= 1;
	this->state[index] = 0;
	this->state[index+1] = 0;
}

bool					Board::is_empty_place(int index) const { return this->filled_pos[index] == 0; }

int						Board::get_player_id(int index) const
{
	index <<= 1;
	if (this->state[index])
		return PLAYER1_ID;
	else if (this->state[index+1])
		return PLAYER2_ID;
	return 0;
}

bool					Board::is_full(void) const { return (this->get_stones_in_play() == BOARDSIZE); }

void					Board::set_state(BITBOARD new_state) { this->state = new_state; }

int						Board::get_stones_in_play(void) const { return PLAYER->stones_in_play + PLAYER->next->stones_in_play; }

Player					*Board::get_player_by_id(int id)
{
	if (id == PLAYER1.id)
		return &PLAYER1;
	
	if (id == PLAYER2.id)
		return &PLAYER2;
	
	return NULL;
}

int						Board::check_captures(int player_id, int index)
{
	int amount = 0;

	for (auto dir : DIRECTIONS)
	{
		if (this->can_capture(player_id, index, dir))
		{
			this->capture(dir, index);
			amount++;
		}
	}
	return amount;
}

bool					Board::is_valid_move(int index, Player &player) const
{
	if (index < 0 || index >= BOARDSIZE)
		return false;
	if (!this->is_empty_place(index))
		return false;
	return true;
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

int						Board::calc_heuristic(void) { return this->heuristic.calc_heuristic(this); }

int						Board::calc_heuristic(Board &node) { return this->heuristic.calc_heuristic(&node); }

void					Board::play(void) { this->play(NULL, NULL); }

void					Board::play(player_fn p1_fn) { this->play(p1_fn, NULL); }

void					Board::play(player_fn p1_fn, player_fn p2_fn)
{
	int index;

	PLAYER = random_int() % 2 == 0 ? PLAYER : PLAYER->next;
	PLAYER1.set_fn(p1_fn);
	PLAYER2.set_fn(p2_fn);
	while (true)
	{
		this->print_stats();
		this->show_last_move();
		std::cout << PLAYER->name << " place: "; 

		index = PLAYER->fn();
		if (index == quit)
			break;
		if (index == error)
			continue;
		if (!this->place(index, PLAYER->id))
			continue;
		if (this->has_won(*PLAYER) || this->is_full())
		{
			this->print_winner();
			break;
		}
		PLAYER = PLAYER->next;
	}
}

/* PRIVATE METHODS */

void					Board::print_winner(void) const
{
	this->print_stats();
	this->show_last_move();
	if (this->is_full())
		std::cout << "*** TIE ***" << std::endl;
	else
		std::cout << "*** " << PLAYER->name << " WINS!!! ***" << std::endl;
}

void					Board::print_stats(void) const
{
	system("clear");
	std::cout << std::endl;
	std::cout << "Captures:" << std::endl;
	std::cout << "\t" << PLAYER1.name << ' ' << PLAYER1.captures;
	std::cout << "\t\t\t" << PLAYER2.name << ' ' << PLAYER2.captures;
	std::cout << std::endl << std::endl;
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

// Assumes that on the given index the correct player is placed
bool					Board::can_capture(int player_id, int index, int dir) const
{
	for (int i = 1; i < 4; i++)
	{
		if (is_offside(index, index + dir))
			break ;
		index += dir;
		if (i == 3 && this->get_player_id(index) == player_id)
			return true;
		else if (this->get_player_id(index) != -player_id)
			break ;
	}
	return false;
}

void					Board::capture(int dir, int index)
{
	for (int i = 1; i < 3; i++)
		this->remove(index + (i * dir));
}

bool					Board::free_threes_direction(int move, int direction, int player) const
{
	int pos;
	int gaps = 0;
	int count = 1;
	int open = 0;
	int shift;
	for (int j = 0; j < 2; j++)
	{
		pos = move;
		shift = DIRECTIONS[direction + 4 * j];
		if (gaps > 1)
			return false;
		for (int i = 0; i < 4; i++)
		{
			pos += shift;
			if (is_offside(pos - shift, pos))
				break;
			if (get_player_id(pos) == player)
				count++;
			else if (get_player_id(pos) == -player)
				break;
			else if (!(is_offside(pos, pos + shift)) && get_player_id(pos + shift) == player)
				gaps++;
			else
			{
				open++;
				break;
			}
		}
	}
	return count == 3 && gaps < 2 && open + gaps > 2;
}

/* OPERATOR OVERLOADS: */

bool 					Board::operator==(Board const &rhs) const { return (this->state == rhs.state); }

bool 					Board::operator!=(Board const &rhs) const { return (this->state != rhs.state); }

bool 					Board::operator==(int const rhs) const { return (this->state == rhs); }

bool 					Board::operator!=(int const rhs) const { return (this->state != rhs); }

BITBOARD				Board::operator&(Board const &rhs) const { return this->state & rhs.state; }

BITBOARD				Board::operator&(BITBOARD const &rhs) const { return this->state & rhs; }

std::ostream			&operator<<(std::ostream &o, Board const &i) { return (o << i.get_state()); }
