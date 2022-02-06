#include "Board.hpp"
#include "misc.hpp"
#include "gomoku.hpp"
#include "heuristic.hpp"
#include "Player.hpp"
#include "IGameEngine.hpp"
#include "TranspositionTable.hpp"

Board::Board(void) :
h(0),
state(0),
filled_pos(0),
winner(-1),
current_player(0),
last_move(-1)
{}

Board::Board(const Board &rhs)
{
	this->h = rhs.h;
	this->filled_pos = rhs.filled_pos;
	PLAYERS[PLAYER1] = rhs.players[PLAYER1];
	PLAYERS[PLAYER2] = rhs.players[PLAYER2];
	this->winner = rhs.winner;
	this->state = rhs.state;
	this->current_player = rhs.current_player;
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
	this->current_player = 0;
	PLAYERS[PLAYER1] = t_player{};
	PLAYERS[PLAYER2] = t_player{};
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

void					Board::show_last_move(void) const { show_move(this->last_move); }

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

bool					Board::place(int row, int col, int player) { return place(misc::calc_index(row, col), player); }

bool					Board::place(int index) { return this->place(index, this->current_player); }

bool					Board::place(int index, int player)
{
	assert(player == PLAYER1 || player == PLAYER2);
	assert(index >= 0 && index < BOARDSIZE);

	if (!is_valid_move(index))
		return false;
	
	this->filled_pos[index] = 1;
	this->last_move = PLAYERS[player].last_move = index;

	this->state[INDEX + player] = 1;
	PLAYERS[player].captures += check_captures(player, index);
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
		board_copy.place(i, this->get_next_player(this->get_last_player()));
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
	int pi = get_player(index);

	this->filled_pos[index] = 0;
	this->state[INDEX + pi] = 0;
}

bool					Board::is_empty_place(int index) const { return this->filled_pos[index] == 0; }

int						Board::get_player(int index) const
{
	index <<= 1;
	if (this->state[index])
		return PLAYER1;
	else if (this->state[index+1])
		return PLAYER2;
	return -1;
}

bool					Board::is_full(void) const { return (total_stones_in_play() == BOARDSIZE); }

int						Board::total_stones_in_play(void) const { return this->filled_pos.count(); }

int						Board::check_captures(int player, int index)
{
	int amount = 0;

	for (auto dir : DIRECTIONS)
	{
		if (can_capture(player, index, dir))
		{
			capture(dir, index);
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

void					Board::print_principal_variation(int player, int depth, TranspositionTable &t_table)
{
	Board node = *this;
	TableEntry tt_entry;

	for (int i = 0; i < depth; i++)
	{
		if (!t_table.lookup(node, tt_entry))
			misc::print_and_quit("can't find node in table! (Print PV)");
		int best_move = tt_entry.best_move;
		PRINT(best_move);
		node.place(best_move, player);
		node.show_last_move();
		
		std::cout << "depth: " << depth << std::endl;
		std::cout << "best move is: " << best_move << std::endl;

		int h = heuristic::get_heuristic_total(node);
		for (uint8_t i = 0; i < 8; i++) // skip first index which is none
		{
			for (int j = 0; j < 2; j++)
			{
				if (i > 0 && node.players[j].patterns[i] > 0)
				{
					PRINT("player: " << j);
					PRINT(PatternNames[i]);
					PRINT((int)node.players[j].patterns[i]);
				}
			}
		}
		std::cout << "heuristic value is: " << h << std::endl;
		best_move = tt_entry.best_move;
		player = 1 - player;
		if(node.is_game_finished())
		{
			PRINT("\n*** game finished ***\n");
			return;
		}
	}
}

void					Board::next_player(void) { this->current_player = (1 - this->current_player); }

int						Board::get_next_player(void) const { return get_next_player(this->current_player); }

int						Board::get_next_player(int player) const { return (1 - player); }

void					Board::play(IGameEngine &engine) { engine.play(this); }

bool					Board::is_game_finished(void) { return is_game_finished(this->current_player); }

bool					Board::is_game_finished(int player)
{
	if (PLAYERS[player].captures >= CAPTUREWIN)
		this->winner = player;
	else if (check_win_other_player(player))
		this->winner = get_next_player(player);
	else if (has_won(player))
		this->winner = player;
	else if (is_full())
		return true;
	return has_winner();
}

bool					Board::check_win_other_player(int player)
{
	int opp_player = get_next_player(player);
	if (PLAYERS[opp_player].wincondition && still_winning(opp_player))
		return true;
	PLAYERS[opp_player].wincondition = false;
	return false;
}

void					Board::random_player(void) { this->current_player = misc::random_int() % 2; }

bool					Board::has_winner(void) const { return (this->winner != -1); }

bool					Board::player_on_index(int index, int player) const { return this->state[INDEX + player]; }

void					Board::set_current_player(int player) { this->current_player = player; }

int						Board::get_current_player(void) { return this->current_player; }

int						Board::get_last_player(void) const { return get_player(this->last_move); }

/* PRIVATE METHODS */

bool					Board::has_won(int player)
{
	if (check_wincondition_all_dir(player))
	{
		PLAYERS[player].wincondition = true;
		return !continue_game(player);
	}
	return false;
}

int						Board::check_wincondition_all_dir(int player) const { return check_wincondition_all_dir(PLAYERS[player].last_move, player); }

int						Board::check_wincondition_all_dir(int index, int player) const
{
	int directions[4] = {DOWN, RIGHT, DIAGDWNL, DIAGDWNR};

	if (is_empty_place(index))
		return 0;
	for (int i = 0; i < 4; i++)
	{
		if (heuristic::count_both_dir(*this, index, player, directions[i]) >= WINCONDITION)
			return directions[i];
	}
	return 0;
}

bool					Board::continue_game(int player)
{
	int op_player = get_next_player(player);
	Board tmp;

	for (int i = 0; i < BOARDSIZE; i++)
	{
		if (!is_empty_place(i))
			continue;
		tmp = *this;
		if ((tmp.check_captures(op_player, i) + PLAYERS[op_player].captures) >= CAPTUREWIN
		|| !tmp.check_wincondition_all_dir(PLAYERS[player].last_move, player))
			return true;
	}
	return false;
}

bool					Board::still_winning(int player) const { return check_wincondition_all_dir(PLAYERS[player].last_move, player); }

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
			if (is_empty_place(n_index) && !misc::is_offside(index, n_index))
				moves[n_index] = 1;
		}
	}
	return moves;
}

bool					Board::can_capture(int player, int index, int dir) const
{
	for (int i = 1; i < 4; i++)
	{
		if (misc::is_offside(index, index + dir))
			break ;
		index += dir;
		if (i == 3 && player_on_index(index, player))
			return true;
		else if (!player_on_index(index, get_next_player(player)))
			break ;
	}
	return false;
}

void					Board::capture(int dir, int index)
{
	for (int i = 1; i < 3; i++)
		remove(index + (i * dir));
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
			if (misc::is_offside(pos - shift, pos))
				break;
			if (get_player(pos) == player)
				count++;
			else if (get_player(pos) == get_next_player(player))
				break;
			else if (!(misc::is_offside(pos, pos + shift)) && get_player(pos + shift) == player)
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
	this->current_player = rhs.current_player;
	this->last_move = rhs.last_move;

	return *this;
}

bool 					Board::operator==(Board const &rhs) const { return (this->state == rhs.state); }

bool 					Board::operator!=(Board const &rhs) const { return (this->state != rhs.state); }

bool 					Board::operator==(int const rhs) const { return (this->state == rhs); }

bool 					Board::operator!=(int const rhs) const { return (this->state != rhs); }

std::ostream			&operator<<(std::ostream &o, Board const &i) { return (o << i.get_state()); }
