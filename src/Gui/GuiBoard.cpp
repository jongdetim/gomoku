#include "GuiBoard.hpp"

GuiBoard::GuiBoard(void) {}

GuiBoard::GuiBoard(const GuiBoard &rhs)
{
	this->board = rhs.board;
	for (int i = 0; i < 2; i++)
	{
		this->players[i] = GuiPlayer(this->board.players[i]);
		this->players[i].name = rhs.players[i].name;
		this->players[i].ai = rhs.players[i].ai;
	}
}

GuiBoard::GuiBoard(Board board) :
	players{
		GuiPlayer(this->board.players[PLAYER1]),
		GuiPlayer(this->board.players[PLAYER2])
	},
	board(board)
{}

Board		&GuiBoard::get_board(void) { return this->board; }

GuiPlayer	GuiBoard::current_player(void) const { return this->players[this->board.get_current_player()]; }

GuiBoard	&GuiBoard::operator=(GuiBoard const &rhs)
{
	this->board = rhs.board;
	for (int i = 0; i < 2; i++)
	{
		this->players[i] = GuiPlayer(this->board.players[i]);
		this->players[i].name = rhs.players[i].name;
		this->players[i].ai = rhs.players[i].ai;
	}
	
	return *this;
}
