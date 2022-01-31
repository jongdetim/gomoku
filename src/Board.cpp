#include "Board.hpp"
#include "misc.hpp"

Board::Board(void) :
h(0), state(0), filled_pos(0), players{Player(1, "P1"), Player(-1, "P2")}, winner(-1), current(0), last_move(-1) {}

Board::Board(const Board &rhs)
{
	this->h = rhs.h;
	this->filled_pos = rhs.filled_pos;
	PLAYERS[PLAYER1] = rhs.players[PLAYER1];
	PLAYERS[PLAYER2] = rhs.players[PLAYER2];
	this->winner = rhs.winner;
	this->state = rhs.state;
	this->current = rhs.current;
	this->last_move = rhs.last_move;
}

Board::~Board() {}

void					Board::reset(void)
{
	this->h = 0;
	this->state.reset();
	this->filled_pos.reset();
	this->last_move = -1;
	this->winner = -1;
	this->current = 0;
	PLAYERS[PLAYER1].reset();
	PLAYERS[PLAYER2].reset();
}

void					Board::print_values(void) const
{
	std::cout << "h        : " << this->h << std::endl;
	std::cout << "winner   : " << this->winner << std::endl;
	std::cout << "lastmove : " << this->last_move << std::endl;
	
	std::cout << "currentP : " << &PLAYERS[current] << std::endl;
	
	std::cout << std::endl << "player1  : " << &PLAYERS[PLAYER1] << std::endl;
	PLAYERS[PLAYER1].print();

	std::cout << std::endl << "player2  : " << &PLAYERS[PLAYER2] << std::endl;
	PLAYERS[PLAYER2].print();
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
			if (is_empty_place(index))
				std::cout << ". ";
			else if (this->state[index<<1])
				std::cout << P1_SYMBOL << ' ';
			else
				std::cout << P2_SYMBOL << ' ';
		}
		std::cout << std::endl;
	}
}

void					Board::show_move(void) const { show_move(this->last_move); }

void					Board::show_move(int show_index) const
{
	std::cout << "   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8" << std::endl;
	for (int row = 0; row < BOARD_LENGTH; row++)
	{
		std::cout << row%10 << ": ";
		for (int col = 0; col < BOARD_LENGTH; col++)
		{
			int index = (row * BOARD_LENGTH + col);
			if (is_empty_place(index))
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

bool					Board::place(int row, int col, int player_index)
{
	return place(misc::calc_index(row, col), player_index);
}

bool					Board::place(int index) { return this->place(index, this->current); }

bool					Board::place(int index, int player_index)
{
	if (!is_valid_move(index))
		return false;
	
	this->filled_pos[index] = 1;
	this->last_move = PLAYERS[player_index].last_move = index;
	PLAYERS[player_index].stones_in_play++;

	this->state[INDEX + player_index] = 1;
	PLAYERS[player_index].captures += check_captures(player_index, index);
	return true;
}

// Add check free threes
bool					Board::is_valid_move(int index) const
{
	return (index >= 0 && index < BOARDSIZE && is_empty_place(index));
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
		board_copy = *this;
		board_copy.place(i);
		nodes.push_back(board_copy);
		// de volgorde hier heeft invloed op de search, ondanks dat deze children nodes nog worden resorteerd. komt dit door gelijke heuristic values en pruning?
		// nodes.insert(nodes.begin(), board_copy);
	}
    return nodes;
}

void					Board::remove(int row, int col) { remove(misc::calc_index(row, col)); }

void					Board::remove(int index)
{
	if (is_empty_place(index))
		return ;
	int pi = get_player_index(index);

	this->filled_pos[index] = 0;
	PLAYERS[pi].stones_in_play--;
	this->state[INDEX + pi] = 0;
}

bool					Board::is_empty_place(int index) const { return this->filled_pos[index] == 0; }

int						Board::get_player_index(int index) const
{
	index <<= 1;
	if (this->state[index])
		return 0;
	else if (this->state[index+1])
		return 1;
	throw "No player present";
	return -1;
}

int						Board::get_player_id(int index) const
{
	index <<= 1;
	if (this->state[index])
		return PLAYER1_ID;
	if (this->state[index+1])
		return PLAYER2_ID;
	return 0;
}

bool					Board::is_full(void) const { return (total_stones_in_play() == BOARDSIZE); }

int						Board::total_stones_in_play(void) const { return PLAYERS[PLAYER1].stones_in_play + PLAYERS[PLAYER2].stones_in_play; }

int						Board::check_captures(int player_index, int index)
{
	int amount = 0;

	for (auto dir : DIRECTIONS)
	{
		if (can_capture(player_index, index, dir))
		{
			capture(dir, index);
			amount++;
		}
	}
	return amount;
}

bool					Board::check_free_threes(int move, int player_id) const
{
	int result = 0;
	// still need to check if last move was NOT a capture
	for (int i = 0; i < 4; i++)
	{
		result += free_threes_direction(move, i, player_id);
		if (result > 1)
			break;
	}
	return result > 1;
}

void					Board::next_player(void) { this->current = (this->current + 1) % 2; }

int						Board::get_next_player_index(void) const { return get_next_player_index(this->current); }

int						Board::get_next_player_index(int player_index) const { return (player_index + 1) % 2; }

void					Board::play(IGameEngine &engine) { engine.play(this); }

bool					Board::is_game_finished(void) { return is_game_finished(this->current); }

bool					Board::is_game_finished(int player_index)
{
	if (PLAYERS[player_index].captures >= CAPTUREWIN)
		this->winner = player_index;
	else if (check_win_other_player(player_index))
		this->winner = get_next_player_index(player_index);
	else if (has_won(PLAYERS[player_index]))
		this->winner = player_index;
	else if (is_full())
		return true;
	return has_winner();
}

bool					Board::check_win_other_player(int player_index)
{
	int opp_player = get_next_player_index(player_index);
	if (PLAYERS[opp_player].has_wincondition() && still_winning(PLAYERS[opp_player]))
		return true;
	PLAYERS[opp_player].winning_index = -1;
	return false;
}

void					Board::random_player(void) { this->current = misc::random_int() % 2; }

bool					Board::has_winner(void) const { return (this->winner != -1); }

bool					Board::player_on_index(int index, int player_index) const { return this->state[INDEX + player_index]; }

void					Board::set_current_player(int player_index) { this->current = player_index; }

Player					*Board::current_player(void) { return &this->players[this->current]; }

/* PRIVATE METHODS */

bool					Board::has_won(Player &player)
{
	if (check_wincodition_all_dir(player.last_move, player.id))
	{
		player.winning_index = player.last_move;
		return !continue_game(player);
	}
	return false;
}

int						Board::check_wincodition_all_dir(int index, int player_id) const { return check_wincodition_all_dir(*this, index, player_id); }

int						Board::check_wincodition_all_dir(const Board &board, int index, int player_id) const
{
	int directions[4] = {DOWN, RIGHT, DIAGDWNL, DIAGDWNR};

	if (is_empty_place(index))
		return 0;
	for (int i = 0; i < 4; i++)
	{
		if (heuristic::count_both_dir(board, index, player_id, directions[i]) >= WINCONDITION)
			return directions[i];
	}
	return 0;
}

bool					Board::continue_game(const Player &player)
{
	int op_player = player.get_next_player_index();
	Board tmp;

	for (int i = 0; i < BOARDSIZE; i++)
	{
		if (!is_empty_place(i))
			continue;
		tmp = *this;
		if ((tmp.check_captures(op_player, i) + PLAYERS[op_player].captures) >= CAPTUREWIN
		|| !check_wincodition_all_dir(tmp, player.last_move, player.id))
			return true;
	}
	return false;
}

bool					Board::still_winning(const Player &player) const { return check_wincodition_all_dir(player.winning_index, player.id); }

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

bool					Board::can_capture(int player_index, int index, int dir) const
{
	for (int i = 1; i < 4; i++)
	{
		if (misc::is_offside(index, index + dir))
			break ;
		index += dir;
		if (i == 3 && player_on_index(index, player_index))
			return true;
		else if (!player_on_index(index, get_next_player_index(player_index)))
			break ;
	}
	return false;
}

void					Board::capture(int dir, int index)
{
	for (int i = 1; i < 3; i++)
		remove(index + (i * dir));
}

bool					Board::free_threes_direction(int move, int direction, int player_id) const
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
			if (misc::is_offside(pos - shift, pos))
				break;
			if (get_player_id(pos) == player_id)
				count++;
			else if (get_player_id(pos) == -player_id)
				break;
			else if (!(misc::is_offside(pos, pos + shift)) && get_player_id(pos + shift) == player_id)
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

Board					&Board::operator=(Board const &rhs)
{
	this->h = rhs.h;
	this->filled_pos = rhs.filled_pos;
	PLAYERS[PLAYER1] = rhs.players[PLAYER1];
	PLAYERS[PLAYER2] = rhs.players[PLAYER2];
	this->winner = rhs.winner;
	this->state = rhs.state;
	this->current = rhs.current;
	this->last_move = rhs.last_move;

	return *this;
}

bool 					Board::operator==(Board const &rhs) const { return (this->state == rhs.state); }

bool 					Board::operator!=(Board const &rhs) const { return (this->state != rhs.state); }

bool 					Board::operator==(int const rhs) const { return (this->state == rhs); }

bool 					Board::operator!=(int const rhs) const { return (this->state != rhs); }

std::ostream			&operator<<(std::ostream &o, Board const &i) { return (o << i.get_state()); }
