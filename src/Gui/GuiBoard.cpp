#include "GuiBoard.hpp"

GuiBoard::GuiBoard(void) {}

GuiBoard::GuiBoard(const GuiBoard &rhs)
{
	this->board = rhs.board;
	this->players[PLAYER1] = GuiPlayer(this->board.players[PLAYER1]);
	this->players[PLAYER2] = GuiPlayer(this->board.players[PLAYER2]);
}


GuiBoard::GuiBoard(Board board) :
board(board), players{GuiPlayer(this->board.players[PLAYER1]), GuiPlayer(this->board.players[PLAYER2])} {}

void		GuiBoard::show_last_move(void) const { this->board.show_last_move(); }

void		GuiBoard::show_move(int index) const { this->board.show_move(index); }

void		GuiBoard::next_player(void) { this->board.next_player(); }

bool		GuiBoard::place(int index) { return this->board.place(index); }

bool		GuiBoard::is_game_finished(void) { return this->board.is_game_finished(); }

bool		GuiBoard::has_winner(void) const { return this->board.has_winner(); }

bool		GuiBoard::is_full(void) const { return this->board.is_full(); }

void		GuiBoard::reset(void) { this->board.reset(); }

void		GuiBoard::random_player(void) { this->board.random_player(); }

int			GuiBoard::get_current_player(void) const { return this->board.get_current_player(); }

int			GuiBoard::get_winner(void) const { return this->board.winner; }

Board		&GuiBoard::get_board(void) { return this->board; }

GuiPlayer	GuiBoard::current_player(void) const { return this->players[this->board.get_current_player()]; }

GuiBoard	&GuiBoard::operator=(GuiBoard const &rhs)
{
	this->board = rhs.board;
	this->players[PLAYER1] = GuiPlayer(this->board.players[PLAYER1]);
	this->players[PLAYER2] = GuiPlayer(this->board.players[PLAYER2]);
	return *this;
}
