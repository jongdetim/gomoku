#include "Player.hpp"
#include "misc.hpp"

Player::Player(int player_id, std::string name, player_fn fn)
{
	assert(player_id == PLAYER1_ID || player_id == PLAYER2_ID);

	this->name = name;
	this->id = player_id;
	this->index_offset = player_id == PLAYER1_ID ? 0 : 1;
	this->fn = fn;
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

bool						Player::has_function(void) const { return this->fn; }

bool						Player::operator==(Player &rhs) const { return this->id == rhs.id; }

bool						Player::operator==(int id) const { return this->id == id; }

bool						Player::operator!=(Player &rhs) const { return  this->id != rhs.id; }

bool						Player::operator!=(int id) const { return  this->id != id; }