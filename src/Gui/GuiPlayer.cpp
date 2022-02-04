#include "GuiPlayer.hpp"


GuiPlayer::GuiPlayer(void) {}

GuiPlayer::GuiPlayer(t_player &player) :
name(""), ai(NULL), player(&player) { }

// GuiPlayer::GuiPlayer(const GuiPlayer &rhs)
// {
// 	this->name = rhs.name;
// 	this->ai = rhs.ai;
// 	// this->player TODO
// }

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

bool						GuiPlayer::wincondition(void) const { return this->player->wincondition; }

// GuiPlayer						&GuiPlayer::operator=(GuiPlayer const &rhs)
// {
// 	this->name = rhs.name;
// 	this->id = rhs.id;
// 	this->index = rhs.index;
// 	this->last_move = rhs.last_move;
// 	this->captures = rhs.captures;
// 	this->stones_in_play = rhs.stones_in_play;
// 	this->winning_index = rhs.winning_index;
// 	this->ai = rhs.ai;
// 	return *this;
// }
