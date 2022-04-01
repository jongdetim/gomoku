/* MIT License

Copyright (c) 2022 Flint Louis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
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
