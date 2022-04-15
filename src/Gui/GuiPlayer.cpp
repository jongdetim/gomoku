/* MIT License

Copyright (c) 2022 Flint Louis, Tim de Jong

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
#include "GuiPlayer.hpp"

GuiPlayer::GuiPlayer(void) {}

GuiPlayer::GuiPlayer(t_player &player) :
name(""),
ai(NULL),
hint_active(false),
player(&player)
{ }

void						GuiPlayer::print(void) const
{
	std::cout << "Name." << this->name << std::endl;
	std::cout << "Captures." << this->player->captures << std::endl;
	std::cout << "LastMove." << this->player->last_move << std::endl;
}

void						GuiPlayer::reset(void)
{
	this->player->last_move = -1;
	this->player->captures = 0;
	this->player->wincondition = false;
	this->hint_active = false;
}

int							GuiPlayer::last_move(void) const { return this->player->last_move; }

int							GuiPlayer::captures(void) const { return this->player->captures; }

bool						GuiPlayer::wincondition(void) const { return this->player->wincondition; }
