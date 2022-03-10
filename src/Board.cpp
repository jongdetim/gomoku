#include "Board.hpp"
#include "misc.hpp"
#include "gomoku.hpp"
#include "heuristic.hpp"
#include "IGameEngine.hpp"
#include "TranspositionTable.hpp"
#include <fstream>

Board::Board(void) :
h(0),
state(0),
filled_pos(0),
winner(-1),
current_player(0),
last_move(-1)
// last_move_was_capture(false)
{}

Board::Board(const Board &rhs)
{
	this->h = rhs.h;
	this->filled_pos = rhs.filled_pos;
	this->players[PLAYER1] = rhs.players[PLAYER1];
	this->players[PLAYER2] = rhs.players[PLAYER2];
	this->winner = rhs.winner;
	this->state = rhs.state;
	this->current_player = rhs.current_player;
	this->last_move = rhs.last_move;
	this->last_move_was_capture = rhs.last_move_was_capture;
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
	this->players[PLAYER1] = t_player{};
	this->players[PLAYER2] = t_player{};
	this->last_move_was_capture = false;
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
			int index = misc::calc_index(row, col);
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
			int index = misc::calc_index(row, col);
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
	this->last_move = this->players[player].last_move = index;

	this->state[INDEX + player] = 1;
	this->players[player].captures += check_captures(player, index);
	return true;
}

void					Board::print_values(void) const
{
	printf("%-*s: %d\n", 10, "h", this->h);
	printf("%-*s: %d\n", 10, "winner", this->winner);
	printf("%-*s: %s\n", 10, "current", this->current_player == PLAYER1 ? "P1" : "P2");
	printf("%-*s: %d\n", 10, "lastMove", this->last_move);
	
	printf("\n");
	printf("            P1   P2\n");
	printf("%-*s: %-*d %d\n", 10, "lastMove", 4, this->players[0].last_move, this->players[1].last_move);
	printf("%-*s: %-*d %d\n", 10, "captures", 4, this->players[0].captures, this->players[1].captures);
	// int p1score = heuristic::score_remaining_patterns(*this, PLAYER1);
	// int p2score = heuristic::score_remaining_patterns(*this, PLAYER2);
	// printf("%-*s: %-*d %d\n", 10, "score", 4, p1score, p2score);
	// p1score = this->current_player == PLAYER1 ? p1score * 1.5 : p1score;
	// p2score = this->current_player == PLAYER2 ? p2score * 1.5 : p2score;
	// printf("%-*s: %-*d %d\n\n", 10, "gameScore", 4, p1score, p2score);
	for (int j = 1; j < 8; j++)
		printf("%-*s: %-*d %d\n", 10, PatternNames[j], 4, this->players[0].patterns[j], this->players[1].patterns[j]);

}

// Add check free threes
bool					Board::is_valid_move(int index) const
{
	return (index >= 0 && index < BOARDSIZE && is_empty_place(index));
}

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

void					Board::remove(int row, int col) { remove(misc::calc_index(row, col)); }

void					Board::remove(int index)
{
	if (is_empty_place(index))
		return ;
	int pi = get_player(index);

	this->filled_pos[index] = 0;
	this->state[INDEX + pi] = 0;
}

bool					Board::is_empty_place(int index) const
{
	assert(index >= 0 && index < BOARDSIZE);
	return this->filled_pos[index] == 0;
}

int						Board::get_player(int index) const
{
	assert(index >= 0 && index < BOARDSIZE);
	index <<= 1;
	if (this->state[index])
		return PLAYER1;
	else if (this->state[index+1])
		return PLAYER2;
	return -1;
}

bool					Board::is_full(void) const { return (total_stones_in_play() == BOARDSIZE); }

bool					Board::is_empty(void) const { return (total_stones_in_play() == 0); }

int						Board::total_stones_in_play(void) const { return this->filled_pos.count(); }

bool					Board::is_capture(int player, int index) const
{
	for (auto dir : DIRECTIONS)
	{
		if (can_capture(player, index, dir))
			return true;
	}
	return false;
}

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
	this->last_move_was_capture = amount;
	return amount;
}

bool					Board::is_free_threes(int move, int player) const
{
	int result = 0;
	if (this->last_move_was_capture)
		return false;
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
		if (!t_table.retrieve(node, tt_entry))
			misc::print_and_quit("can't find node in table! (Print PV)");
		int best_move = tt_entry.best_move;
		PRINT(best_move);
		node.place(best_move, player);
		node.show_last_move();
		
		std::cout << "depth: " << depth << std::endl;
		std::cout << "best move is: " << best_move << std::endl;

		int h = heuristic::get_heuristic_total(node, player);
		for (uint8_t i = 1; i < 8; i++) // skip first index which is none
		{
			for (int j = 0; j < 2; j++)
			{
				if (node.players[j].patterns[i] > 0)
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

void					Board::print_players_patterns(void) const
{		
	PRINT("player0       player1");
	for (uint8_t pattern = 1; pattern < 8; pattern++)
	{
		printf("%-*s: %-*d ", 8, PatternNames[pattern], 3, (int)this->players[0].patterns[pattern]);
		printf("%-*s: %d\n", 8, PatternNames[pattern], (int)this->players[1].patterns[pattern]);
	}
}

void					Board::print_player_patterns(int player) const
{
	PRINT("player" << player);
	for (uint8_t pattern = 1; pattern < 8; pattern++) // skip first index which is none
		printf("%-*s: %d\n", 8, PatternNames[pattern], (int)this->players[player].patterns[pattern]);
}

void					Board::next_player(void) { this->current_player = (1 - this->current_player); }

int						Board::get_next_player(void) const { return get_next_player(this->current_player); }

int						Board::get_next_player(int player) const { return (1 - player); }

bool					Board::is_game_finished(void) { return is_game_finished(this->current_player); }

bool					Board::is_game_finished(int player)
{
	if (this->players[player].captures >= CAPTUREWIN)
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
	if (this->players[opp_player].wincondition && still_winning(opp_player))
		return true;
	this->players[opp_player].wincondition = false;
	return false;
}

void					Board::random_player(void) { this->current_player = misc::get_random_int() % 2; }

bool					Board::has_winner(void) const { return (this->winner != -1); }

bool					Board::player_on_index(int index, int player) const { return this->state[INDEX + player]; }

void					Board::set_current_player(int player) { this->current_player = player; }

int						Board::get_current_player(void) const { return this->current_player; }

int						Board::get_last_player(void) const { return get_player(this->last_move); }

void					Board::save(std::string file_name) const
{
	std::ofstream file;
	file.open(file_name);
	file.write((char*)this,sizeof(*this));
	file.close();
}

void					Board::load(std::string file_name)
{
    Board board;

    std::ifstream file;
    file.open(file_name, std::ios::in);
	if (file.fail())
		throw "failed to load board state from file";
    file.seekg(0);
    file.read((char*)&board, sizeof(board));
    file.close();

	*this = board;
}

/* PRIVATE METHODS */

bool					Board::has_won(int player)
{
	if (check_wincondition_all_dir(player))
	{
		this->players[player].wincondition = true;
		return !continue_game(player);
	}
	return false;
}

int						Board::check_wincondition_all_dir(int player) const { return check_wincondition_all_dir(this->players[player].last_move, player); }

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
		if ((tmp.check_captures(op_player, i) + this->players[op_player].captures) >= CAPTUREWIN
		|| !tmp.check_wincondition_all_dir(this->players[player].last_move, player))
			return true;
	}
	return false;
}

bool					Board::still_winning(int player) const { return check_wincondition_all_dir(this->players[player].last_move, player); }

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
			if (!misc::is_offside(index, n_index) && is_empty_place(n_index))
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
	int total_open = 0;
	int shift;
	for (int j = 0; j < 2; j++)
	{
		int open = 0;
		pos = move;
		shift = DIRECTIONS[direction + 4 * j];
		if (gaps > 1)
			return false;
		for (int i = 0; i < 4; i++)
		{
			pos += shift;
			if (misc::is_offside(pos - shift, pos) || get_player(pos) == get_next_player(player))
				break;
			else if (get_player(pos) == player)
				count++;
			else if (open == 0 && !(misc::is_offside(pos, pos + shift)) && get_player(pos + shift) == player)
				gaps++;
			else
			{
				open++;
				if (open == 2)
					break;
			}
		}
		if (open == 0)
			return false;
		total_open += open;
	}
	return count == 3 && gaps < 2 && total_open + gaps > 2;
}

/* OPERATOR OVERLOADS: */

Board					&Board::operator=(Board const &rhs)
{
	this->h = rhs.h;
	this->filled_pos = rhs.filled_pos;
	this->players[PLAYER1] = rhs.players[PLAYER1];
	this->players[PLAYER2] = rhs.players[PLAYER2];
	this->winner = rhs.winner;
	this->state = rhs.state;
	this->current_player = rhs.current_player;
	this->last_move = rhs.last_move;
	this->last_move_was_capture = rhs.last_move_was_capture;

	return *this;
}

bool 					Board::operator==(Board const &rhs) const { return (this->state == rhs.state); }

bool 					Board::operator!=(Board const &rhs) const { return (this->state != rhs.state); }

bool 					Board::operator==(int const rhs) const { return (this->state == rhs); }

bool 					Board::operator!=(int const rhs) const { return (this->state != rhs); }

std::ostream			&operator<<(std::ostream &o, Board const &i) { return (o << i.get_state()); }
