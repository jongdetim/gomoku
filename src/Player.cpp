#include "Player.hpp"
#include "misc.hpp"


// GuiPlayer::GuiPlayer(t_player &player) :
// name(""),
// ai(NULL)
// {
// 	this->player = &player;
// }

// GuiPlayer::GuiPlayer(const GuiPlayer &rhs)
// {
// 	this->name = rhs.name;
// 	this->ai = rhs.ai;
// 	// this->player TODO
// }

// void						GuiPlayer::print(void) const
// {
// 	std::cout << "Name." << this->name << std::endl;
// 	std::cout << "Captures." << this->player->captures << std::endl;
// 	std::cout << "LastMove." << this->player->last_move << std::endl;
// 	std::cout << "StonesInPlay." << this->stones_in_play << std::endl;
// }

// void						GuiPlayer::reset(void)
// {
// 	this->last_move = -1;
// 	this->captures = 0;
// 	this->stones_in_play = 0;
// 	this->winning_index = -1;
// }

// bool						GuiPlayer::has_wincondition(void) const { return this->winning_index != -1; }

// int							GuiPlayer::get_next_player_index(void) const { return (this->index + 1) % 2; }

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
