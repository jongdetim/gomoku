#include "Player.hpp"
#include "misc.hpp"


Player::Player(void) : next(NULL), ai(NULL) {}

Player::Player(const Player &rhs)
{
	this->name = rhs.name;
	this->id = rhs.id;
	this->last_move = rhs.last_move;
	this->captures = rhs.captures;
	this->stones_in_play = rhs.stones_in_play;
	this->winning_index = rhs.winning_index;
	this->index_offset = rhs.index_offset;
	this->next = NULL;
	this->ai = rhs.ai;
}

Player::Player(int player_id, std::string name) : next(NULL), ai(NULL)
{
	assert(player_id == PLAYER1_ID || player_id == PLAYER2_ID);

	this->name = name;
	this->id = player_id;
	this->index_offset = player_id == PLAYER1_ID ? 0 : 1;
	this->reset();
}

void						Player::print(void) const
{
	std::cout << "Name." << this->name << std::endl;
	std::cout << "Id." << this->id << std::endl;
	std::cout << "IndexOffset." << this->index_offset << std::endl;
	std::cout << "Captures." << this->captures << std::endl;
	std::cout << "LastMove." << this->last_move << std::endl;
	std::cout << "StonesInPlay." << this->stones_in_play << std::endl;
	std::cout << "Next." << this->next << std::endl;
}

void						Player::reset(void)
{
	this->last_move = -1;
	this->captures = 0;
	this->stones_in_play = 0;
	this->winning_index = -1;
}

int							Player::index(int index) const { return (index << 1) + this->index_offset; }

bool						Player::has_wincondition(void) const { return this->winning_index != -1; }

Player						&Player::operator=(Player const &rhs)
{
	this->name = rhs.name;
	this->id = rhs.id;
	this->last_move = rhs.last_move;
	this->captures = rhs.captures;
	this->stones_in_play = rhs.stones_in_play;
	this->winning_index = rhs.winning_index;
	this->index_offset = rhs.index_offset;
	this->next = NULL;
	this->ai = rhs.ai;
	return *this;
}

bool						Player::operator==(Player &rhs) const { return this->id == rhs.id; }

bool						Player::operator==(int id) const { return this->id == id; }

bool						Player::operator!=(Player &rhs) const { return  this->id != rhs.id; }

bool						Player::operator!=(int id) const { return  this->id != id; }
