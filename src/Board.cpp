#include "Board.hpp"
#include "misc.hpp"

Board::Board(void) : h(0), state(0), filled_pos(0), player1(PLAYER1_ID, "P1"), player2(PLAYER2_ID, "P2"), winner(0)
{
	PLAYER1.next = &PLAYER2;
	PLAYER2.next = &PLAYER1;
	PLAYER = &PLAYER1;
}

Board::~Board() {}

void					Board::reset(void)
{
	this->h = 0;
	this->state.reset();
	this->last_move = -1;
	this->winner = 0;
	PLAYER1.reset();
	PLAYER2.reset();
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
				std::cout << P1_SYMBOL << ' ';
			else
				std::cout << P2_SYMBOL << ' ';
		}
		std::cout << std::endl;
	}
}

void					Board::show_move(void) const { this->show_move(this->last_move); }

void					Board::show_move(int show_index) const
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
				if (show_index == index)
					printf("%s%c %s", CYAN, P1_SYMBOL-32, DEFAULT);
				else
					std::cout << P1_SYMBOL << ' ';
			}
			else
			{
				if (show_index == index)
					printf("%s%c %s", RED, P2_SYMBOL-32, DEFAULT);
				else
					std::cout << P2_SYMBOL << ' ';
			}
		}
		std::cout << std::endl;
	}
}

bool					Board::place(int row, int col, int player_id)
{
	this->switch_to_player(player_id);
	return this->place(calc_index(row, col), *PLAYER);
}

bool					Board::place(int index, int player_id)
{
	this->switch_to_player(player_id);
	return this->place(index, *PLAYER);
}

bool					Board::place(int index) { return this->place(index, *PLAYER); }

bool					Board::place(int index, Player &player)
{
	if (!this->is_valid_move(index, player))
		return false;
	
	this->filled_pos[index] = 1;
	this->last_move = player.last_move = index;
	player.stones_in_play++;

	this->state[player.index(index)] = 1;
	player.captures += this->check_captures(player, index);
	return true;
}

bool					Board::is_valid_move(int index, Player &player) const
{
	if (index < 0 || index >= BOARDSIZE)
		return false;
	if (!this->is_empty_place(index))
		return false;
	return true;
}

std::vector<Board>		Board::generate_children(void) const
{
	Board board_copy;
    std::vector<Board> nodes;

	auto moves = get_moves();
	for (int i = 0; i < moves.size(); i++)
	{
		if (!moves[i])
			continue;
		board_copy = this->get_copy();
		board_copy.place(i);
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
	auto p = this->get_player_by_id(this->get_player_id(index));
	p->stones_in_play--;
	this->state[p->index(index)] = 0;
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

bool					Board::is_full(void) const { return (this->total_stones_in_play() == BOARDSIZE); }

int						Board::total_stones_in_play(void) const { return PLAYER->stones_in_play + PLAYER->next->stones_in_play; }

Player					*Board::get_player_by_id(int id)
{
	assert(id == PLAYER1_ID || id == PLAYER2_ID);

	if (id == PLAYER1.id)
		return &PLAYER1;
	return &PLAYER2;
	
}

int						Board::check_captures(Player &player, int index)
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

void					Board::next_player(void) { PLAYER = PLAYER->next; }

void					Board::switch_to_player(int id)
{
	if (id == PLAYER1_ID)
		PLAYER = &PLAYER1;
	else if (id == PLAYER2_ID)
		PLAYER = &PLAYER2;
}

void					Board::play(IGameEngine &engine) { engine.play(this); }

bool					Board::is_game_finished(Player &player)
{
	if (player.captures >= CAPTUREWIN)
		this->winner = player.id;
	else if (this->check_win_other_player(player))
		this->winner = player.next->id;
	else if (this->has_won(player))
		this->winner = player.id;
	else if (this->is_full())
		return true;
	return this->winner != 0;
}

bool					Board::check_win_other_player(Player &player) const { return (player.next->has_wincondition() && this->still_winning(*player.next)); }

bool					Board::has_won(void) const { return this->heuristic.has_won(this, *PLAYER); }

bool					Board::has_won(Player &player) const { return this->heuristic.has_won(this, player); }

Board					Board::get_copy(void) const
{
	Board copy =  *this;
	copy.current_player = copy.get_player_by_id(PLAYER->id);
	copy.player1.next = &copy.player2;
	copy.player2.next = &copy.player1;

	return copy;
}

void					Board::random_player(void) { PLAYER = random_int() % 2 == 0 ? PLAYER : PLAYER->next; }

/* PRIVATE METHODS */

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

bool					Board::can_capture(Player &player, int index, int dir) const
{
	for (int i = 1; i < 4; i++)
	{
		if (is_offside(index, index + dir))
			break ;
		index += dir;
		if (i == 3 && player == this->get_player_id(index))
			return true;
		else if (this->get_player_id(index) != -player.id)
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

bool					Board::still_winning(Player &player) const { return this->heuristic.still_winning(this, player); }

/* OPERATOR OVERLOADS: */

bool 					Board::operator==(Board const &rhs) const { return (this->state == rhs.state); }

bool 					Board::operator!=(Board const &rhs) const { return (this->state != rhs.state); }

bool 					Board::operator==(int const rhs) const { return (this->state == rhs); }

bool 					Board::operator!=(int const rhs) const { return (this->state != rhs); }

std::ostream			&operator<<(std::ostream &o, Board const &i) { return (o << i.get_state()); }
