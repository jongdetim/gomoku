#include "GuiPlayer.hpp"


GuiPlayer::GuiPlayer(void) {}

GuiPlayer::GuiPlayer(t_player &player) :
name(""), ai(NULL), player(&player) { }

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
}

int							GuiPlayer::last_move(void) const { return this->player->last_move; }

int							GuiPlayer::captures(void) const { return this->player->captures; }

int							GuiPlayer::stones(void) const { return this->player->stones; }

bool						GuiPlayer::wincondition(void) const { return this->player->wincondition; }
