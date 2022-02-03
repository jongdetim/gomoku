#include "Player.hpp"
#include "misc.hpp"


Player::Player(void) :
name(""), id(0), index(-1), last_move(-1), captures(0), stones_in_play(0), winning_index(-1), ai(NULL) {}

Player::Player(const Player &rhs)
{
	this->name = rhs.name;
	this->id = rhs.id;
	this->index = rhs.index;
	this->last_move = rhs.last_move;
	this->captures = rhs.captures;
	this->stones_in_play = rhs.stones_in_play;
	this->winning_index = rhs.winning_index;
	this->ai = rhs.ai;
}

Player::Player(int player_id, std::string name) :
name(name), id(player_id), index((player_id == PLAYER1_ID ? PLAYER1_IDX : PLAYER2_IDX)), last_move(-1), captures(0), stones_in_play(0), winning_index(-1), ai(NULL) {}

void						Player::print(void) const
{
	std::cout << "Name." << this->name << std::endl;
	std::cout << "Id." << this->id << std::endl;
	std::cout << "Captures." << this->captures << std::endl;
	std::cout << "LastMove." << this->last_move << std::endl;
	std::cout << "StonesInPlay." << this->stones_in_play << std::endl;
}

void						Player::reset(void)
{
	this->last_move = -1;
	this->captures = 0;
	this->stones_in_play = 0;
	this->winning_index = -1;
}

bool						Player::has_wincondition(void) const { return this->winning_index != -1; }

int							Player::get_next_player_index(void) const { return (this->index + 1) % 2; }

Player						&Player::operator=(Player const &rhs)
{
	this->name = rhs.name;
	this->id = rhs.id;
	this->index = rhs.index;
	this->last_move = rhs.last_move;
	this->captures = rhs.captures;
	this->stones_in_play = rhs.stones_in_play;
	this->winning_index = rhs.winning_index;
	this->ai = rhs.ai;
	return *this;
}

bool						Player::operator==(Player &rhs) const { return this->id == rhs.id; }

bool						Player::operator==(int id) const { return this->id == id; }

bool						Player::operator!=(Player &rhs) const { return  this->id != rhs.id; }

bool						Player::operator!=(int id) const { return  this->id != id; }
